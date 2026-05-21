#include "puttyalt_minimap.h"
#include <string.h>
#include <stdlib.h>

int minimap_init(MinimapState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int minimap_update(MinimapState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void minimap_destroy(MinimapState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
