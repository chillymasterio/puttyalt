/* puttyalt_schedule2.c - Task scheduler v2 with priorities + dependencies. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SC_MAX 32
#define SC_NAME 48
typedef struct { char name[SC_NAME]; uint64_t run_at_ms; int priority; int dep_mask; int done; int running; } sc_task;
typedef struct { sc_task tasks[SC_MAX]; int n; } Schedule2;
void schedule2_init(Schedule2 *s) { if(s) memset(s,0,sizeof(*s)); }
int schedule2_add(Schedule2 *s, const char *name, uint64_t run_at_ms, int priority, int dep_mask) {
    if(!s||s->n>=SC_MAX||!name) return -1;
    sc_task *t=&s->tasks[s->n]; snprintf(t->name,SC_NAME,"%s",name); t->run_at_ms=run_at_ms; t->priority=priority; t->dep_mask=dep_mask;
    return s->n++;
}
static int sc_done_mask(const Schedule2 *s) {
    int m=0; for(int i=0;i<s->n;i++) if(s->tasks[i].done) m|=(1<<i); return m;
}
int schedule2_next(Schedule2 *s, uint64_t now_ms) {
    if(!s) return -1; int done=sc_done_mask(s); int best=-1;
    for (int i=0;i<s->n;i++) {
        sc_task *t=&s->tasks[i];
        if (t->done || t->running) continue;
        if (now_ms < t->run_at_ms) continue;
        if ((t->dep_mask & done) != t->dep_mask) continue; /* deps not satisfied */
        if (best<0 || t->priority>s->tasks[best].priority) best=i;
    }
    if (best>=0) s->tasks[best].running=1;
    return best;
}
int schedule2_complete(Schedule2 *s, int idx) {
    if(!s||idx<0||idx>=s->n) return -1; s->tasks[idx].done=1; s->tasks[idx].running=0; return 0;
}
int schedule2_pending(const Schedule2 *s) {
    if(!s) return -1; int n=0; for(int i=0;i<s->n;i++) if(!s->tasks[i].done)n++; return n;
}
