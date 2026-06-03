/* puttyalt_obsv.c - Observability: metric counters + gauge + histogram buckets. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define OB_MAX 32
#define OB_NAME 40
#define OB_BUCKETS 8
enum ob_type { OB_COUNTER=0, OB_GAUGE=1, OB_HISTO=2 };
typedef struct { char name[OB_NAME]; int type; int64_t value; int64_t buckets[OB_BUCKETS]; } ob_metric;
typedef struct { ob_metric m[OB_MAX]; int n; } Observ;
void obsv_init(Observ *o) { if(o) memset(o,0,sizeof(*o)); }
static ob_metric *obsv_find(Observ *o, const char *name, int type) {
    for (int i=0;i<o->n;i++) if (strcmp(o->m[i].name,name)==0) return &o->m[i];
    if (o->n>=OB_MAX) return NULL;
    ob_metric *m=&o->m[o->n++]; snprintf(m->name,OB_NAME,"%s",name); m->type=type; return m;
}
int obsv_incr(Observ *o, const char *name, int64_t by) {
    if(!o||!name) return -1; ob_metric *m=obsv_find(o,name,OB_COUNTER); if(!m) return -1; m->value+=by; return 0;
}
int obsv_set_gauge(Observ *o, const char *name, int64_t v) {
    if(!o||!name) return -1; ob_metric *m=obsv_find(o,name,OB_GAUGE); if(!m) return -1; m->value=v; return 0;
}
int obsv_observe(Observ *o, const char *name, int64_t val) {
    if(!o||!name) return -1; ob_metric *m=obsv_find(o,name,OB_HISTO); if(!m) return -1;
    int b=0; int64_t t=1; while (b<OB_BUCKETS-1 && val>=t) { t*=10; b++; }
    m->buckets[b]++; m->value++; return 0;
}
int64_t obsv_get(const Observ *o, const char *name) {
    if(!o||!name) return -1;
    for (int i=0;i<o->n;i++) if (strcmp(o->m[i].name,name)==0) return o->m[i].value;
    return -1;
}
int obsv_export(const Observ *o, char *buf, int buflen) {
    if(!o||!buf) return -1; int pos=0;
    for (int i=0;i<o->n && pos<buflen;i++) pos+=snprintf(buf+pos,buflen-pos,"%s %lld\n",o->m[i].name,(long long)o->m[i].value);
    return pos;
}
