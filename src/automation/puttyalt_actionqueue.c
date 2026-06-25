/* puttyalt_actionqueue.c - Queue and dedupe pending actions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define AQ_CAP 64
typedef struct { char action[48]; int priority; } AqItem;
typedef struct { AqItem items[AQ_CAP]; int n; } ActionQueue;
void aq2_init(ActionQueue *q) { if (q) q->n = 0; }
/* Enqueue, skipping if an identical action is already pending. */
int aq2_enqueue(ActionQueue *q, const char *action, int priority) {
    if (!q || !action || q->n >= AQ_CAP) return -1;
    for (int i = 0; i < q->n; i++) if (strcmp(q->items[i].action, action) == 0) return 1; /* dup */
    strncpy(q->items[q->n].action, action, 47); q->items[q->n].action[47] = 0;
    q->items[q->n].priority = priority;
    q->n++;
    return 0;
}
/* Dequeue the highest-priority action. */
int aq2_dequeue(ActionQueue *q, char *out, int outlen) {
    if (!q || q->n == 0 || !out) return -1;
    int best = 0;
    for (int i = 1; i < q->n; i++) if (q->items[i].priority > q->items[best].priority) best = i;
    strncpy(out, q->items[best].action, outlen-1); out[outlen-1] = 0;
    for (int i = best; i < q->n - 1; i++) q->items[i] = q->items[i+1];
    q->n--;
    return 0;
}
