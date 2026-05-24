#include "puttyalt_reconnect_v2.h"
#include <string.h>
#include <stdlib.h>

int reconnect_v2_init(Reconnect_v2State *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int reconnect_v2_update(Reconnect_v2State *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void reconnect_v2_destroy(Reconnect_v2State *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
