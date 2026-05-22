#ifndef PUTTYALT_DNS_CACHE_H
#define PUTTYALT_DNS_CACHE_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Dns_cacheState;

int  dns_cache_init(Dns_cacheState *s);
int  dns_cache_update(Dns_cacheState *s);
void dns_cache_destroy(Dns_cacheState *s);

#endif
