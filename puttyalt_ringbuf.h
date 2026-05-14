#ifndef PUTTYALT_RINGBUF_H
#define PUTTYALT_RINGBUF_H
#include "puttyalt_termemu.h"

typedef struct {
    TermCell **lines;
    int cols;
    int capacity;
    int count;
    int head;
} RingBuffer;

int  ring_init(RingBuffer *rb, int capacity, int cols);
void ring_destroy(RingBuffer *rb);
void ring_push(RingBuffer *rb, const TermCell *line);
TermCell *ring_get(RingBuffer *rb, int index);
int  ring_count(RingBuffer *rb);
void ring_clear(RingBuffer *rb);
int  ring_search(RingBuffer *rb, const char *text, int start_line, int *out_line, int *out_col);

#endif
