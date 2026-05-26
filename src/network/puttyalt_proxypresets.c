#include <string.h>
#include <stdio.h>
#define MAX_PRESETS 32
typedef struct { char name[64]; char host[256]; int port; int type; char user[64]; } ProxyPreset;
static ProxyPreset g_presets[MAX_PRESETS]; static int g_preset_count = 0;
int proxypreset_add(const char *name, const char *host, int port, int type) {
    if (g_preset_count >= MAX_PRESETS) return -1;
    ProxyPreset *p = &g_presets[g_preset_count]; memset(p, 0, sizeof(*p));
    snprintf(p->name, 64, "%s", name); snprintf(p->host, 256, "%s", host);
    p->port = port; p->type = type; return g_preset_count++;
}
ProxyPreset *proxypreset_find(const char *name) {
    for (int i = 0; i < g_preset_count; i++) if (strcmp(g_presets[i].name, name) == 0) return &g_presets[i];
    return NULL;
}
int proxypreset_remove(const char *name) {
    for (int i = 0; i < g_preset_count; i++) if (strcmp(g_presets[i].name, name) == 0) {
        memmove(&g_presets[i], &g_presets[i+1], sizeof(ProxyPreset)*(g_preset_count-i-1));
        g_preset_count--; return 0;
    }
    return -1;
}
int proxypreset_count(void) { return g_preset_count; }
