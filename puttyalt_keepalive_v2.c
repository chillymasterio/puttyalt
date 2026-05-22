#include "puttyalt_keepalive_v2.h"
#include <string.h>
#include <stdlib.h>

int keepalive_v2_init(Keepalive_v2State *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int keepalive_v2_update(Keepalive_v2State *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void keepalive_v2_destroy(Keepalive_v2State *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
