/* puttyalt_preset2.c - Connection preset v2 with parameter overrides. */
#include <string.h>
#include <stdio.h>
#define PR_MAX 16
#define PR_NAME 40
typedef struct { char name[PR_NAME]; int compression; int keepalive_sec; char cipher_pref[64]; int x11_forward; int agent_forward; } pr_preset;
typedef struct { pr_preset presets[PR_MAX]; int n; int active; } Preset2;
void preset2_init(Preset2 *p) { if(p){ memset(p,0,sizeof(*p)); p->active=-1; } }
int preset2_create(Preset2 *p, const char *name) {
    if(!p||p->n>=PR_MAX||!name) return -1;
    pr_preset *pr=&p->presets[p->n]; snprintf(pr->name,PR_NAME,"%s",name); pr->keepalive_sec=30; return p->n++;
}
int preset2_set_options(Preset2 *p, int idx, int compression, int keepalive, int x11, int agent) {
    if(!p||idx<0||idx>=p->n) return -1;
    pr_preset *pr=&p->presets[idx]; pr->compression=compression?1:0; pr->keepalive_sec=keepalive;
    pr->x11_forward=x11?1:0; pr->agent_forward=agent?1:0; return 0;
}
int preset2_activate(Preset2 *p, const char *name) {
    if(!p||!name) return -1;
    for (int i=0;i<p->n;i++) if (strcmp(p->presets[i].name,name)==0) { p->active=i; return i; }
    return -1;
}
const pr_preset *preset2_active(const Preset2 *p) {
    return (p&&p->active>=0&&p->active<p->n)?&p->presets[p->active]:0;
}
int preset2_count(const Preset2 *p) { return p?p->n:-1; }
