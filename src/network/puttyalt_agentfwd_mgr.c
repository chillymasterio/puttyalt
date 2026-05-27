#include <string.h>
#include <stdio.h>
#define MAX_FORWARDED_KEYS 32
typedef struct { char fingerprint[128]; char comment[256]; int forwarded; int locked; } AgentKey;
static AgentKey g_keys[MAX_FORWARDED_KEYS]; static int g_key_count = 0;
int agentfwd_add_key(const char *fp, const char *comment) {
    if (g_key_count >= MAX_FORWARDED_KEYS) return -1;
    AgentKey *k = &g_keys[g_key_count]; memset(k, 0, sizeof(*k));
    snprintf(k->fingerprint, 128, "%s", fp);
    if (comment) snprintf(k->comment, 256, "%s", comment);
    k->forwarded = 1; return g_key_count++;
}
int agentfwd_lock_key(int idx) { if (idx<0||idx>=g_key_count) return -1; g_keys[idx].locked=1; return 0; }
int agentfwd_is_forwarded(const char *fp) {
    for (int i=0;i<g_key_count;i++) if (strcmp(g_keys[i].fingerprint,fp)==0) return g_keys[i].forwarded;
    return 0;
}
int agentfwd_count(void) { return g_key_count; }
