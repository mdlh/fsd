#include <fsd/drm_connect.h>
#include "drm_connection.h"

fsd_drm_connection fsd_drm_connect (const char * path)
{
  return _drm_connection_create ();
}

void fsd_drm_disconnect (fsd_drm_connection session)
{
  _drm_connection_destroy (session);
}
