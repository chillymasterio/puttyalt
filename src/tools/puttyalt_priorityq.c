/* puttyalt_priorityq.c - Priority queue with explicit priorities.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define PQ_CAP 128
typedef struct { int value; int priority; } PqItem;
typedef struct { PqItem heap[PQ_CAP]; int n; } PriorityQ;
void pq2_init(PriorityQ *q) { if (q) q->n = 0; }
int pq2_push(PriorityQ *q, int value, int priority) {
    if (!q || q->n >= PQ_CAP) return -1;
    int i = q->n++;
    q->heap[i].value = value; q->heap[i].priority = priority;
    while (i > 0) {
        int p = (i-1)/2;
        if (q->heap[p].priority <= q->heap[i].priority) break;
        PqItem t = q->heap[p]; q->heap[p] = q->heap[i]; q->heap[i] = t; i = p;
    }
    return 0;
}
int pq2_pop(PriorityQ *q, int *value) {
    if (!q || q->n == 0) return -1;
    if (value) *value = q->heap[0].value;
    q->heap[0] = q->heap[--q->n];
    int i = 0;
    while (1) {
        int l = 2*i+1, r = 2*i+2, m = i;
        if (l < q->n && q->heap[l].priority < q->heap[m].priority) m = l;
        if (r < q->n && q->heap[r].priority < q->heap[m].priority) m = r;
        if (m == i) break;
        PqItem t = q->heap[m]; q->heap[m] = q->heap[i]; q->heap[i] = t; i = m;
    }
    return 0;
}
