/* puttyalt_taskqueue.c - Priority task queue for background automation jobs. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TQ_MAX 64
#define TQ_DESC 96
typedef struct { char desc[TQ_DESC]; int priority; uint64_t enqueued_ms; int id; } tq_task;
typedef struct { tq_task t[TQ_MAX]; int n; int next_id; } TaskQueue;
void taskqueue_init(TaskQueue *q) { if(q){ memset(q,0,sizeof(*q)); q->next_id=1; } }
int taskqueue_push(TaskQueue *q, const char *desc, int priority, uint64_t now_ms) {
    if(!q||q->n>=TQ_MAX) return -1;
    tq_task *t=&q->t[q->n++]; snprintf(t->desc,TQ_DESC,"%s",desc?desc:""); t->priority=priority;
    t->enqueued_ms=now_ms; t->id=q->next_id++; return t->id;
}
int taskqueue_pop(TaskQueue *q, char *desc, int desclen) {
    if(!q||q->n==0) return -1;
    int best=0;
    for (int i=1;i<q->n;i++)
        if (q->t[i].priority>q->t[best].priority ||
           (q->t[i].priority==q->t[best].priority && q->t[i].enqueued_ms<q->t[best].enqueued_ms)) best=i;
    int id=q->t[best].id;
    if (desc) snprintf(desc,desclen,"%s",q->t[best].desc);
    memmove(&q->t[best],&q->t[best+1],sizeof(tq_task)*(q->n-best-1)); q->n--;
    return id;
}
int taskqueue_size(const TaskQueue *q) { return q?q->n:-1; }
