#include "puttyalt_connpool.h"
#include <string.h>
#include <stdlib.h>

int connpool_init(ConnpoolState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int connpool_update(ConnpoolState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void connpool_destroy(ConnpoolState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
