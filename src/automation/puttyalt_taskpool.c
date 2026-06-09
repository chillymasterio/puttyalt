/* puttyalt_taskpool.c - Worker task pool with priority + concurrency limit. */
#include <string.h>
#include <stdio.h>
#define TP_MAX 64
#define TP_DESC 64
enum tp_state { TP_QUEUED=0, TP_RUNNING=1, TP_DONE=2, TP_FAILED=3 };
typedef struct { char desc[TP_DESC]; int priority; int state; } tp_task;
typedef struct { tp_task tasks[TP_MAX]; int n; int max_concurrent; int running; int completed; } TaskPool;
void taskpool_init(TaskPool *t, int max_concurrent) {
    if(!t) return; memset(t,0,sizeof(*t)); t->max_concurrent=max_concurrent>0?max_concurrent:4;
}
int taskpool_submit(TaskPool *t, const char *desc, int priority) {
    if(!t||t->n>=TP_MAX) return -1;
    tp_task *task=&t->tasks[t->n]; snprintf(task->desc,TP_DESC,"%s",desc?desc:""); task->priority=priority; task->state=TP_QUEUED;
    return t->n++;
}
int taskpool_dispatch(TaskPool *t) {
    if(!t||t->running>=t->max_concurrent) return -1;
    int best=-1;
    for (int i=0;i<t->n;i++) if (t->tasks[i].state==TP_QUEUED) {
        if (best<0||t->tasks[i].priority>t->tasks[best].priority) best=i;
    }
    if (best<0) return -1;
    t->tasks[best].state=TP_RUNNING; t->running++; return best;
}
int taskpool_complete(TaskPool *t, int idx, int success) {
    if(!t||idx<0||idx>=t->n) return -1;
    t->tasks[idx].state=success?TP_DONE:TP_FAILED; t->running--; t->completed++; return 0;
}
int taskpool_queued(const TaskPool *t) {
    if(!t) return -1; int n=0; for(int i=0;i<t->n;i++) if(t->tasks[i].state==TP_QUEUED)n++; return n;
}
int taskpool_running(const TaskPool *t) { return t?t->running:-1; }
