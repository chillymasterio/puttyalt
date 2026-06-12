/* puttyalt_pipeline2.c - Pipeline v2: DAG of command stages with dependency gating. */
#include <string.h>
#include <stdio.h>
#define PL_MAX 24
#define PL_CMD 160
enum pl_state { PL_WAIT=0, PL_READY, PL_RUN, PL_OK, PL_FAIL };
typedef struct { char cmd[PL_CMD]; int deps_mask; int state; } pl_stage;
typedef struct { pl_stage s[PL_MAX]; int n; } Pipeline2;
void pipeline2_init(Pipeline2 *p) { if(p) memset(p,0,sizeof(*p)); }
int pipeline2_add(Pipeline2 *p, const char *cmd, int deps_mask) {
    if(!p||p->n>=PL_MAX) return -1;
    snprintf(p->s[p->n].cmd,PL_CMD,"%s",cmd?cmd:""); p->s[p->n].deps_mask=deps_mask; p->s[p->n].state=PL_WAIT;
    return p->n++;
}
static int pl_done_mask(const Pipeline2 *p) {
    int m=0; for (int i=0;i<p->n;i++) if (p->s[i].state==PL_OK) m|=(1<<i); return m;
}
int pipeline2_next_ready(Pipeline2 *p) {
    if(!p) return -1;
    int done=pl_done_mask(p);
    for (int i=0;i<p->n;i++) if (p->s[i].state==PL_WAIT && (p->s[i].deps_mask & done)==p->s[i].deps_mask) {
        p->s[i].state=PL_RUN; return i;
    }
    return -1;
}
int pipeline2_complete(Pipeline2 *p, int idx, int success) {
    if(!p||idx<0||idx>=p->n) return -1;
    p->s[idx].state = success?PL_OK:PL_FAIL;
    if (!success) { for (int i=0;i<p->n;i++) if ((p->s[i].deps_mask&(1<<idx)) && p->s[i].state==PL_WAIT) p->s[i].state=PL_FAIL; }
    return 0;
}
int pipeline2_all_done(const Pipeline2 *p) {
    if(!p) return -1;
    for(int i=0;i<p->n;i++) if(p->s[i].state==PL_WAIT||p->s[i].state==PL_RUN||p->s[i].state==PL_READY) return 0; return 1;
}
int pipeline2_failures(const Pipeline2 *p) { if(!p) return -1; int n=0; for(int i=0;i<p->n;i++) if(p->s[i].state==PL_FAIL)n++; return n; }
