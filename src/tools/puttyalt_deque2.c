/* puttyalt_deque2.c - Double-ended queue of ints.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define DQ_CAP 256
typedef struct { int data[DQ_CAP]; int head, tail, count; } Deque2;
void dq2_init(Deque2 *d) { if (d) { d->head = 0; d->tail = 0; d->count = 0; } }
int dq2_push_front(Deque2 *d, int v) {
    if (!d || d->count >= DQ_CAP) return -1;
    d->head = (d->head - 1 + DQ_CAP) % DQ_CAP;
    d->data[d->head] = v; d->count++;
    return 0;
}
int dq2_push_back(Deque2 *d, int v) {
    if (!d || d->count >= DQ_CAP) return -1;
    d->data[d->tail] = v; d->tail = (d->tail + 1) % DQ_CAP; d->count++;
    return 0;
}
int dq2_pop_front(Deque2 *d, int *out) {
    if (!d || d->count == 0) return -1;
    if (out) *out = d->data[d->head];
    d->head = (d->head + 1) % DQ_CAP; d->count--;
    return 0;
}
int dq2_pop_back(Deque2 *d, int *out) {
    if (!d || d->count == 0) return -1;
    d->tail = (d->tail - 1 + DQ_CAP) % DQ_CAP;
    if (out) *out = d->data[d->tail];
    d->count--;
    return 0;
}
int dq2_size(const Deque2 *d) { return d ? d->count : 0; }
