/* puttyalt_workflowstep.c - Multi-host workflow step executor state. */
#include <string.h>
#include <stdio.h>
#define WS_MAX_HOSTS 32
#define WS_NAME 64
enum ws_state { WS_PENDING=0, WS_RUNNING, WS_OK, WS_FAIL };
typedef struct { char host[WS_NAME]; int state; int exit_code; } ws_host;
typedef struct { ws_host h[WS_MAX_HOSTS]; int n; char command[256]; int parallel; int fail_fast; } WorkflowStep;
void workflowstep_init(WorkflowStep *w, const char *command, int parallel, int fail_fast) {
    if(!w) return; memset(w,0,sizeof(*w)); snprintf(w->command,256,"%s",command?command:"");
    w->parallel=parallel?1:0; w->fail_fast=fail_fast?1:0;
}
int workflowstep_add_host(WorkflowStep *w, const char *host) {
    if(!w||w->n>=WS_MAX_HOSTS||!host) return -1;
    snprintf(w->h[w->n].host,WS_NAME,"%s",host); return w->n++;
}
int workflowstep_next(WorkflowStep *w) {
    if(!w) return -1;
    for (int i=0;i<w->n;i++) if (w->h[i].state==WS_PENDING) {
        if (!w->parallel) { /* serial: only advance if no running */
            for (int j=0;j<w->n;j++) if (w->h[j].state==WS_RUNNING) return -1;
        }
        w->h[i].state=WS_RUNNING; return i;
    }
    return -1;
}
int workflowstep_complete(WorkflowStep *w, int idx, int exit_code) {
    if(!w||idx<0||idx>=w->n) return -1;
    w->h[idx].exit_code=exit_code; w->h[idx].state=exit_code==0?WS_OK:WS_FAIL;
    if (exit_code!=0 && w->fail_fast) {
        for (int i=0;i<w->n;i++) if (w->h[i].state==WS_PENDING) w->h[i].state=WS_FAIL;
        return 1; /* aborted remaining */
    }
    return 0;
}
int workflowstep_done(const WorkflowStep *w) {
    if(!w) return -1; for(int i=0;i<w->n;i++) if(w->h[i].state==WS_PENDING||w->h[i].state==WS_RUNNING) return 0; return 1;
}
int workflowstep_failures(const WorkflowStep *w) { if(!w) return -1; int n=0; for(int i=0;i<w->n;i++) if(w->h[i].state==WS_FAIL)n++; return n; }
