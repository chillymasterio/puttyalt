/* puttyalt_pluginhost.c - Plugin host runtime with lifecycle + sandboxing. */
#include <string.h>
#include <stdio.h>
#define PH_MAX 16
#define PH_NAME 48
enum ph_state { PH_UNLOADED=0, PH_LOADED=1, PH_ACTIVE=2, PH_SUSPENDED=3, PH_ERROR=4 };
typedef struct { char name[PH_NAME]; int state; int capabilities; int api_version; int error_count; } ph_plugin;
typedef struct { ph_plugin plugins[PH_MAX]; int n; int host_api_version; } PluginHost;
void pluginhost_init(PluginHost *h, int api_version) {
    if(!h) return; memset(h,0,sizeof(*h)); h->host_api_version=api_version;
}
int pluginhost_load(PluginHost *h, const char *name, int api_version, int capabilities) {
    if(!h||h->n>=PH_MAX||!name) return -1;
    if (api_version>h->host_api_version) return -2; /* incompatible */
    ph_plugin *p=&h->plugins[h->n]; snprintf(p->name,PH_NAME,"%s",name); p->state=PH_LOADED;
    p->api_version=api_version; p->capabilities=capabilities; return h->n++;
}
int pluginhost_activate(PluginHost *h, int idx) {
    if(!h||idx<0||idx>=h->n||h->plugins[idx].state!=PH_LOADED) return -1;
    h->plugins[idx].state=PH_ACTIVE; return 0;
}
int pluginhost_has_capability(const PluginHost *h, int idx, int cap) {
    if(!h||idx<0||idx>=h->n) return 0; return (h->plugins[idx].capabilities&cap)?1:0;
}
int pluginhost_report_error(PluginHost *h, int idx) {
    if(!h||idx<0||idx>=h->n) return -1;
    if (++h->plugins[idx].error_count>=3) { h->plugins[idx].state=PH_ERROR; return 1; }
    return 0;
}
int pluginhost_active_count(const PluginHost *h) {
    if(!h) return -1; int n=0; for(int i=0;i<h->n;i++) if(h->plugins[i].state==PH_ACTIVE)n++; return n;
}
