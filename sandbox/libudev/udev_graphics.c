#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *entry;
	struct udev_device *dev;

	/*
	 * Initialize Udev
	 */
	udev = udev_new ();
	if (!udev) {
		fprintf(stderr, "Cannot create udev\n");
		exit(1);
	}

	/*
	 * Prepare enumeration
	 */
	enumerate = udev_enumerate_new (udev);
	udev_enumerate_add_match_subsystem (enumerate, "drm");
	udev_enumerate_scan_devices (enumerate);
	devices = udev_enumerate_get_list_entry (enumerate);

	/*
	 * Iterate over enumeration results
	 */
	udev_list_entry_foreach (entry, devices) {
		const char *sys_path, *node_path, *dev_type, *minor_value;
		struct udev_list_entry * property;

		sys_path = udev_list_entry_get_name (entry);

		dev = udev_device_new_from_syspath (udev, sys_path);
		node_path = udev_device_get_devnode (dev);

		minor_value = udev_device_get_property_value (dev, "MINOR");

		if ( !minor_value
                     || (strlen (minor_value) != 1)
                     || minor_value[0] != '0' ) {
			struct udev_device * parent = 
				udev_device_get_parent_with_subsystem_devtype (dev, "drm", NULL);
			if ( parent ) {
				const char * connect_name;
				const char * parent_path;
				parent_path = udev_device_get_syspath (parent);
				connect_name = sys_path;
				connect_name += strlen (parent_path) + 1;
				printf("Potential connector name: %s\n", connect_name);
			}
			udev_device_unref (dev);
			continue;
		}

		printf ("Dri path: %s\n", node_path);

		udev_device_unref (dev);
	}

	udev_enumerate_unref (enumerate);
	udev_unref (udev);
	return 0;
}
