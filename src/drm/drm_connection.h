#ifndef _DRM_CONNECTION_H
#define _DRM_CONNECTION_H

struct _fsd_drm_connection {
  int fd;
};

struct _fsd_drm_connection * _drm_connection_create ();
void _drm_connection_destroy (struct _fsd_drm_connection * session);

#endif /* _DRM_CONNECTION_H */
