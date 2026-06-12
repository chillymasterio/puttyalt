/* puttyalt_reflowcache.c - Cache of computed line-wrap break points per width. */
#include <string.h>
#include <stdio.h>
#define RC_MAX_LINES 256
#define RC_MAX_BREAKS 16
typedef struct { int line_id; int width; int breaks[RC_MAX_BREAKS]; int nbreaks; int valid; } rc_entry;
typedef struct { rc_entry e[RC_MAX_LINES]; int n; int hits, misses; } ReflowCache;
void reflowcache_init(ReflowCache *c) { if(c) memset(c,0,sizeof(*c)); }
int reflowcache_lookup(ReflowCache *c, int line_id, int width, int *breaks, int cap) {
    if(!c) return -1;
    for (int i=0;i<c->n;i++) if (c->e[i].valid && c->e[i].line_id==line_id && c->e[i].width==width) {
        c->hits++;
        int k=c->e[i].nbreaks<cap?c->e[i].nbreaks:cap;
        if (breaks) memcpy(breaks,c->e[i].breaks,sizeof(int)*k);
        return k;
    }
    c->misses++; return -1;
}
int reflowcache_store(ReflowCache *c, int line_id, int width, const int *breaks, int nbreaks) {
    if(!c||nbreaks<0||nbreaks>RC_MAX_BREAKS) return -1;
    rc_entry *e=NULL;
    for (int i=0;i<c->n;i++) if (c->e[i].line_id==line_id) { e=&c->e[i]; break; }
    if (!e) { if(c->n>=RC_MAX_LINES) return -1; e=&c->e[c->n++]; }
    e->line_id=line_id; e->width=width; e->nbreaks=nbreaks; e->valid=1;
    if (breaks) memcpy(e->breaks,breaks,sizeof(int)*nbreaks);
    return 0;
}
void reflowcache_invalidate_line(ReflowCache *c, int line_id) {
    if(!c) return;
    for (int i=0;i<c->n;i++) if (c->e[i].line_id==line_id) c->e[i].valid=0;
}
void reflowcache_invalidate_all(ReflowCache *c) {
    if(!c) return;
    for (int i=0;i<c->n;i++) c->e[i].valid=0;
}
int reflowcache_hit_rate_pct(const ReflowCache *c) {
    if(!c) return -1;
    int t=c->hits+c->misses; return t? (c->hits*100)/t : 0;
}
