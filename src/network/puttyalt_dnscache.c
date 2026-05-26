#include <string.h>
#include <stdio.h>
#include <time.h>
#define DNS_CACHE_MAX 256
typedef struct { char hostname[256]; char ip[64]; time_t resolved_at; int ttl; } DnsEntry;
static DnsEntry g_dns[DNS_CACHE_MAX]; static int g_dns_count = 0;
int dnscache_put(const char *host, const char *ip, int ttl) {
    for (int i = 0; i < g_dns_count; i++)
        if (strcmp(g_dns[i].hostname, host) == 0) {
            snprintf(g_dns[i].ip, 64, "%s", ip); g_dns[i].resolved_at = time(NULL); g_dns[i].ttl = ttl; return 0;
        }
    if (g_dns_count >= DNS_CACHE_MAX) return -1;
    DnsEntry *e = &g_dns[g_dns_count++]; memset(e, 0, sizeof(*e));
    snprintf(e->hostname, 256, "%s", host); snprintf(e->ip, 64, "%s", ip);
    e->resolved_at = time(NULL); e->ttl = ttl; return 0;
}
const char *dnscache_get(const char *host) {
    time_t now = time(NULL);
    for (int i = 0; i < g_dns_count; i++)
        if (strcmp(g_dns[i].hostname, host) == 0 && (now - g_dns[i].resolved_at) < g_dns[i].ttl)
            return g_dns[i].ip;
    return NULL;
}
void dnscache_flush(void) { g_dns_count = 0; }
int dnscache_count(void) { return g_dns_count; }
