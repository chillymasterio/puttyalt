/* puttyalt_ringqueue.c - Fixed-capacity ring queue of integer tokens. */
#include <string.h>
#define RQ_CAP 256
typedef struct {
    int data[RQ_CAP];
    int head, tail, count;
    int overflow;
} RingQueue;
void ringqueue_init(RingQueue *q) {
    if (q) memset(q, 0, sizeof(*q));
}
int ringqueue_push(RingQueue *q, int value) {
    if (!q) return -1;
    if (q->count >= RQ_CAP) {
        q->overflow++;
        q->tail = (q->tail + 1) % RQ_CAP;
        q->count--;
    }
    q->data[q->head] = value;
    q->head = (q->head + 1) % RQ_CAP;
    q->count++;
    return 0;
}
int ringqueue_pop(RingQueue *q, int *out) {
    if (!q || q->count == 0) return -1;
    if (out) *out = q->data[q->tail];
    q->tail = (q->tail + 1) % RQ_CAP;
    q->count--;
    return 0;
}
int ringqueue_peek(const RingQueue *q, int *out) {
    if (!q || q->count == 0) return -1;
    if (out) *out = q->data[q->tail];
    return 0;
}
int ringqueue_size(const RingQueue *q) {
    return q ? q->count : -1;
}
int ringqueue_overflow(const RingQueue *q) {
    return q ? q->overflow : -1;
}
