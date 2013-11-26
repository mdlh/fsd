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

	/* Prepare monitor and subscribe before enumeration */
	struct udev_monitor * mon;
	mon = udev_monitor_new_from_netlink (udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype (mon, "input", NULL);
	udev_monitor_enable_receiving (mon);

	/*
	 * Prepare enumeration
	 */
	enumerate = udev_enumerate_new (udev);
	udev_enumerate_add_match_subsystem (enumerate, "input");
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

		printf ("sys path: %s\n", sys_path);

		udev_device_unref (dev);
	}

	udev_enumerate_unref (enumerate);

	while (1) {
		dev = udev_monitor_receive_device (mon);
		if ( dev ) {
			printf ("---------------------");
			printf ("%s\n", udev_device_get_syspath (dev));
			printf ("Device: %s\n", udev_device_get_devnode (dev));
			printf ("Action: %s\n", udev_device_get_action (dev));
			udev_device_unref (dev);
		}
	}
	udev_unref (udev);
	return 0;
}
