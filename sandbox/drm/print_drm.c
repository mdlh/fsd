#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xf86drmMode.h>

static const char * drm_node = "/dev/dri/card0";

static int open_drm ()
{
	int fd_drm;
	
	fd_drm = open(drm_node, O_RDWR | O_CLOEXEC);

	if ( fd_drm < 0 ) {
		perror ("Cannot open drm");
		exit (1);
	}

	return fd_drm;
}

const char * conn_type_strings [] =
{
	"Unknown",
	"VGA",
	"DVII",
	"DVID",
	"DVIA",
	"Composite",
	"SVIDEO",
	"LVDS",
	"Component",
	"9PinDIN",
	"DisplayPort",
	"HDMIA",
	"HDMIB",
	"TV",
	"eDP"
};

enum {
	max_conn_type_id = sizeof(conn_type_strings)/sizeof(const char *)
};

static void scan_connectors ( int drm_fd )
{
	drmModeRes * res;
	drmModeConnector *conn;
	unsigned int i;

	res = drmModeGetResources ( drm_fd );
	if ( !res ) {
		perror ("Cannot retrieve DRM resources ");
		return;
	}

	for ( i = 0; i < res->count_connectors; ++i ) {
		int prop, num_props;
		conn = drmModeGetConnector ( drm_fd, res->connectors[i] );
		if ( ! conn ) {
			perror ( "Cannot retrieve DRM connector");
			continue;
		}
		num_props = conn->count_props;
		printf("Connector id(%u), type(%s), # properties (%u)\n",
			conn->connector_id,
			conn_type_strings[conn->connector_type],
			conn->count_props);

		drmModeObjectPropertiesPtr props_info;		
		props_info = drmModeObjectGetProperties (drm_fd,
							 conn->connector_id,
							 DRM_MODE_OBJECT_CONNECTOR);
		for ( prop = 0; prop < num_props; ++prop ) {
			printf ("Prop %d type: %u, value: %lu\n", prop, conn->props[prop], conn->prop_values[prop]);
			drmModePropertyPtr property = drmModeGetProperty(drm_fd, conn->props[prop]);
			if (!property) {
				printf ("Could not get property\n");
				continue;
			}
			printf("Name: %s\n", property->name);
		}

		drmModeFreeConnector ( conn );
	}

}

int main (int argc, char ** argv)
{
	int fd = open_drm ();
	scan_connectors ( fd );

	close (fd);
	return 0;
}
