#include "puttyalt_ringbuf.h"
#include <stdlib.h>
#include <string.h>

int ring_init(RingBuffer *rb, int capacity, int cols)
{
    memset(rb, 0, sizeof(*rb));
    rb->cols = cols;
    rb->capacity = capacity > 0 ? capacity : 10000;
    rb->lines = calloc(rb->capacity, sizeof(TermCell *));
    return rb->lines ? 0 : -1;
}

void ring_destroy(RingBuffer *rb)
{
    if (rb->lines) {
        for (int i = 0; i < rb->capacity; i++) free(rb->lines[i]);
        free(rb->lines);
    }
    memset(rb, 0, sizeof(*rb));
}

void ring_push(RingBuffer *rb, const TermCell *line)
{
    int idx = (rb->head + rb->count) % rb->capacity;
    if (rb->count >= rb->capacity) {
        free(rb->lines[rb->head]);
        rb->head = (rb->head + 1) % rb->capacity;
    } else {
        rb->count++;
    }
    rb->lines[idx] = malloc(rb->cols * sizeof(TermCell));
    if (rb->lines[idx]) memcpy(rb->lines[idx], line, rb->cols * sizeof(TermCell));
}

TermCell *ring_get(RingBuffer *rb, int index)
{
    if (index < 0 || index >= rb->count) return NULL;
    return rb->lines[(rb->head + index) % rb->capacity];
}

int ring_count(RingBuffer *rb) { return rb->count; }

void ring_clear(RingBuffer *rb)
{
    for (int i = 0; i < rb->capacity; i++) { free(rb->lines[i]); rb->lines[i] = NULL; }
    rb->count = 0; rb->head = 0;
}

int ring_search(RingBuffer *rb, const char *text, int start_line, int *out_line, int *out_col)
{
    int tlen = (int)strlen(text);
    if (tlen == 0) return -1;
    for (int i = start_line; i < rb->count; i++) {
        TermCell *line = ring_get(rb, i);
        if (!line) continue;
        for (int c = 0; c <= rb->cols - tlen; c++) {
            int match = 1;
            for (int j = 0; j < tlen && match; j++) {
                if ((char)line[c + j].ch != text[j]) match = 0;
            }
            if (match) { *out_line = i; *out_col = c; return 0; }
        }
    }
    return -1;
}
