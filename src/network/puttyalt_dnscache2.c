/* puttyalt_dnscache2.c - DNS cache v2 with negative caching + TTL. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define DC_MAX 64
#define DC_HOST 96
#define DC_ADDR 48
typedef struct {
    char host[DC_HOST];
    char addr[DC_ADDR];
    uint64_t expires_ms;
    int negative;
} dc_entry;
typedef struct {
    dc_entry entries[DC_MAX];
    int n;
    int hits;
    int misses;
} DnsCache2;
void dnscache2_init(DnsCache2 *d) {
    if (d) memset(d, 0, sizeof(*d));
}
int dnscache2_put(DnsCache2 *d, const char *host, const char *addr, int ttl_sec, uint64_t now_ms) {
    if (!d || !host) return -1;
    dc_entry *e = NULL;
    for (int i = 0; i < d->n; i++) {
        if (strcmp(d->entries[i].host, host) == 0) {
            e = &d->entries[i];
            break;
        }
    }
    if (!e) {
        if (d->n >= DC_MAX) {
            int oldest = 0;
            for (int i = 1; i < d->n; i++)
                if (d->entries[i].expires_ms < d->entries[oldest].expires_ms) oldest = i;
            e = &d->entries[oldest];
        } else {
            e = &d->entries[d->n++];
        }
    }
    snprintf(e->host, DC_HOST, "%s", host);
    snprintf(e->addr, DC_ADDR, "%s", addr ? addr : "");
    e->negative = (addr == NULL);
    e->expires_ms = now_ms + (uint64_t)ttl_sec * 1000;
    return 0;
}
int dnscache2_get(DnsCache2 *d, const char *host, uint64_t now_ms, char *out, int outlen) {
    if (!d || !host) return -1;
    for (int i = 0; i < d->n; i++) {
        if (strcmp(d->entries[i].host, host) == 0) {
            if (now_ms >= d->entries[i].expires_ms) {
                d->misses++;
                return -1;
            }
            d->hits++;
            if (d->entries[i].negative) return -2;
            if (out) snprintf(out, outlen, "%s", d->entries[i].addr);
            return 0;
        }
    }
    d->misses++;
    return -1;
}
int dnscache2_hit_rate(const DnsCache2 *d) {
    if (!d) return -1;
    int total = d->hits + d->misses;
    return total ? d->hits * 100 / total : 0;
}
int dnscache2_count(const DnsCache2 *d) {
    return d ? d->n : -1;
}
