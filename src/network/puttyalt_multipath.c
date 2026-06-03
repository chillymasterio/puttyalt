/* puttyalt_multipath.c - Multipath transport: per-path scheduling + failover. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define MP_MAX 8
#define MP_NAME 48
enum mp_state { MP_DOWN=0, MP_UP=1, MP_BACKUP=2 };
typedef struct { char iface[MP_NAME]; int state; int rtt_ms; int weight; uint64_t bytes; } mp_path;
typedef struct { mp_path p[MP_MAX]; int n; int active; } MultiPath;
void multipath_init(MultiPath *m) { if(m){ memset(m,0,sizeof(*m)); m->active=-1; } }
int multipath_add(MultiPath *m, const char *iface, int weight) {
    if(!m||m->n>=MP_MAX||!iface) return -1;
    mp_path *p=&m->p[m->n]; snprintf(p->iface,MP_NAME,"%s",iface); p->weight=weight>0?weight:1; p->state=MP_UP; p->rtt_ms=1000;
    if (m->active<0) m->active=m->n;
    return m->n++;
}
void multipath_update(MultiPath *m, int idx, int up, int rtt_ms) {
    if(!m||idx<0||idx>=m->n) return;
    m->p[idx].state = up?MP_UP:MP_DOWN; if (up) m->p[idx].rtt_ms=rtt_ms;
}
int multipath_select(MultiPath *m) {
    if(!m) return -1;
    /* pick lowest-rtt UP path */
    int best=-1, br=1<<30;
    for (int i=0;i<m->n;i++) if (m->p[i].state==MP_UP && m->p[i].rtt_ms<br) { br=m->p[i].rtt_ms; best=i; }
    m->active=best; return best;
}
int multipath_failover(MultiPath *m) {
    if(!m||m->active<0) return -1;
    m->p[m->active].state=MP_DOWN; return multipath_select(m);
}
void multipath_account(MultiPath *m, int idx, uint64_t bytes) { if(m&&idx>=0&&idx<m->n) m->p[idx].bytes+=bytes; }
int multipath_up_count(const MultiPath *m) { if(!m) return -1; int n=0; for(int i=0;i<m->n;i++) if(m->p[i].state==MP_UP)n++; return n; }
