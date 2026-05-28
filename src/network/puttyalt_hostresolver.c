#include <string.h>
#include <stdio.h>
#define MAX_RESOLVE 64
typedef struct { char hostname[256]; char ipv4[64]; char ipv6[128]; int resolved; long resolve_time_ms; } ResolveResult;
static ResolveResult g_cache[MAX_RESOLVE]; static int g_cache_count = 0;
int resolver_cache_lookup(const char *host, char *ip, int iplen) {
    for (int i=0;i<g_cache_count;i++) if (strcmp(g_cache[i].hostname,host)==0 && g_cache[i].resolved) {
        snprintf(ip, iplen, "%s", g_cache[i].ipv4[0] ? g_cache[i].ipv4 : g_cache[i].ipv6); return 0;
    }
    return -1;
}
int resolver_cache_store(const char *host, const char *ipv4, const char *ipv6, long ms) {
    if (g_cache_count >= MAX_RESOLVE) g_cache_count = 0;
    ResolveResult *r = &g_cache[g_cache_count++]; memset(r, 0, sizeof(*r));
    snprintf(r->hostname, 256, "%s", host);
    if (ipv4) snprintf(r->ipv4, 64, "%s", ipv4);
    if (ipv6) snprintf(r->ipv6, 128, "%s", ipv6);
    r->resolved = 1; r->resolve_time_ms = ms; return 0;
}
void resolver_cache_flush(void) { g_cache_count = 0; }
int resolver_cache_count(void) { return g_cache_count; }
