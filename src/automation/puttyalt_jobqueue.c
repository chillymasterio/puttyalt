/* puttyalt_jobqueue.c - FIFO job queue with status tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define JQ_CAP 128
enum { JQ_PENDING, JQ_RUNNING, JQ_DONE, JQ_FAILED };
typedef struct { int id; int status; } JqJob;
typedef struct { JqJob job[JQ_CAP]; int head, tail, n; int next_id; } JobQueue;
void jq2_init(JobQueue *q) { if (q) { q->head = q->tail = q->n = 0; q->next_id = 1; } }
int jq2_submit(JobQueue *q) {
    if (!q || q->n >= JQ_CAP) return -1;
    int id = q->next_id++;
    q->job[q->tail].id = id; q->job[q->tail].status = JQ_PENDING;
    q->tail = (q->tail + 1) % JQ_CAP; q->n++;
    return id;
}
int jq2_take(JobQueue *q) {
    if (!q) return -1;
    for (int k = 0, i = q->head; k < q->n; k++, i = (i+1)%JQ_CAP)
        if (q->job[i].status == JQ_PENDING) { q->job[i].status = JQ_RUNNING; return q->job[i].id; }
    return -1;
}
int jq2_finish(JobQueue *q, int id, int ok) {
    if (!q) return -1;
    for (int k = 0, i = q->head; k < q->n; k++, i = (i+1)%JQ_CAP)
        if (q->job[i].id == id) { q->job[i].status = ok ? JQ_DONE : JQ_FAILED; return 0; }
    return -1;
}
