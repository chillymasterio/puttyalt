/* puttyalt_notifystack2.c - Vertical stack of transient notifications. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

typedef struct notifystack2_ring {
    uint32_t *data;
    int cap, head, len;
} notifystack2_ring;

int notifystack2_init(notifystack2_ring *r, uint32_t *storage, int cap) {
    if (!r || !storage || cap <= 0) return -1;
    r->data = storage;
    r->cap = cap;
    r->head = 0;
    r->len = 0;
    return 0;
}
int notifystack2_push(notifystack2_ring *r, uint32_t v) {
    if (!r || !r->data) return -1;
    r->data[(r->head + r->len) % r->cap] = v;
    if (r->len < r->cap) r->len++;
    else r->head = (r->head + 1) % r->cap;
    return 0;
}
int notifystack2_pop(notifystack2_ring *r, uint32_t *out) {
    if (!r || !r->data || r->len == 0) return -1;
    if (out) *out = r->data[r->head];
    r->head = (r->head + 1) % r->cap;
    r->len--;
    return 0;
}
