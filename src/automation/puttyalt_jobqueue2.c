/* puttyalt_jobqueue2.c - Job queue v2 with dependencies + priority. */
#include <string.h>
#include <stdio.h>
#define JQ_MAX 48
#define JQ_NAME 40
enum jq_state { JQ_WAIT = 0, JQ_READY = 1, JQ_RUN = 2, JQ_DONE = 3, JQ_FAIL = 4 };
typedef struct {
    char name[JQ_NAME];
    int priority;
    int dep_mask;
    int state;
} jq_job;
typedef struct {
    jq_job jobs[JQ_MAX];
    int n;
} JobQueue2;
void jobqueue2_init(JobQueue2 *q) {
    if (q) memset(q, 0, sizeof(*q));
}
int jobqueue2_add(JobQueue2 *q, const char *name, int priority, int dep_mask) {
    if (!q || q->n >= JQ_MAX || !name) return -1;
    jq_job *j = &q->jobs[q->n];
    snprintf(j->name, JQ_NAME, "%s", name);
    j->priority = priority;
    j->dep_mask = dep_mask;
    j->state = JQ_WAIT;
    return q->n++;
}
static int jq_done_mask(const JobQueue2 *q) {
    int m = 0;
    for (int i = 0; i < q->n; i++)
        if (q->jobs[i].state == JQ_DONE) m |= (1 << i);
    return m;
}
int jobqueue2_next(JobQueue2 *q) {
    if (!q) return -1;
    int done = jq_done_mask(q);
    int best = -1;
    for (int i = 0; i < q->n; i++) {
        if (q->jobs[i].state != JQ_WAIT) continue;
        if ((q->jobs[i].dep_mask & done) != q->jobs[i].dep_mask) continue;
        if (best < 0 || q->jobs[i].priority > q->jobs[best].priority) best = i;
    }
    if (best >= 0) q->jobs[best].state = JQ_RUN;
    return best;
}
int jobqueue2_complete(JobQueue2 *q, int idx, int success) {
    if (!q || idx < 0 || idx >= q->n) return -1;
    q->jobs[idx].state = success ? JQ_DONE : JQ_FAIL;
    return 0;
}
int jobqueue2_pending(const JobQueue2 *q) {
    if (!q) return -1;
    int n = 0;
    for (int i = 0; i < q->n; i++)
        if (q->jobs[i].state == JQ_WAIT || q->jobs[i].state == JQ_RUN) n++;
    return n;
}
