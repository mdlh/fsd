#ifndef _DRM_CONNECT_H
#define _DRM_CONNECT_H

/* opaque handler for a drm session */
typedef struct _fsd_drm_connection * fsd_drm_connection;

/* create a drm session */
fsd_drm_connection fsd_drm_connect (const char * path);

/* close the drm session */
void fsd_drm_disconnect (fsd_drm_connection session);

#endif  /* _DRM_CONNECT_H */
