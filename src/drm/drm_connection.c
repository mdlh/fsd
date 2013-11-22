#include "drm_connection.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct _fsd_drm_connection * _drm_connection_create ()
{
        struct _fsd_drm_connection *result;

        result = (struct _fsd_drm_connection *) malloc (sizeof (*result));
        if (!result) return NULL;

        memset (result, 0, sizeof (*result));

        return result;
}

void _drm_connection_destroy (struct _fsd_drm_connection * session)
{
        assert (session && "Null Ptr session provided");
        free (session);
}
