#include "puttyalt_dns_cache.h"
#include <string.h>
#include <stdlib.h>

int dns_cache_init(Dns_cacheState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int dns_cache_update(Dns_cacheState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void dns_cache_destroy(Dns_cacheState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
