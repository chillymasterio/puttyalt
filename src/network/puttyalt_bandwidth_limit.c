#include "puttyalt_bandwidth_limit.h"
#include <string.h>
#include <stdlib.h>

int bandwidth_limit_init(Bandwidth_limitState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int bandwidth_limit_update(Bandwidth_limitState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void bandwidth_limit_destroy(Bandwidth_limitState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
