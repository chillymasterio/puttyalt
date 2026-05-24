#include "puttyalt_proxy_detect.h"
#include <string.h>
#include <stdlib.h>

int proxy_detect_init(Proxy_detectState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int proxy_detect_update(Proxy_detectState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void proxy_detect_destroy(Proxy_detectState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
