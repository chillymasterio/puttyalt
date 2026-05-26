#include <string.h>
#include <stdio.h>
#define MAX_KNOWN 512
typedef struct { char host[256]; char fingerprint[128]; int trusted; long first_seen; } KnownHost;
static KnownHost g_known[MAX_KNOWN]; static int g_known_count = 0;
int hostverify_check(const char *host, const char *fp) {
    for (int i = 0; i < g_known_count; i++)
        if (strcmp(g_known[i].host, host) == 0) return strcmp(g_known[i].fingerprint, fp) == 0 ? 1 : -1;
    return 0; /* unknown */
}
int hostverify_trust(const char *host, const char *fp, long ts) {
    if (g_known_count >= MAX_KNOWN) return -1;
    KnownHost *k = &g_known[g_known_count++]; memset(k, 0, sizeof(*k));
    snprintf(k->host, 256, "%s", host); snprintf(k->fingerprint, 128, "%s", fp);
    k->trusted = 1; k->first_seen = ts; return 0;
}
int hostverify_revoke(const char *host) {
    for (int i = 0; i < g_known_count; i++) if (strcmp(g_known[i].host, host) == 0) {
        memmove(&g_known[i], &g_known[i+1], sizeof(KnownHost)*(g_known_count-i-1));
        g_known_count--; return 0;
    }
    return -1;
}
int hostverify_count(void) { return g_known_count; }
