/* puttyalt_celldiff.c - Terminal cell diff tracker for minimal screen updates. */
#include <string.h>
#include <stdint.h>
#define CD_MAX 4096
typedef struct {
    uint32_t prev[CD_MAX];
    uint32_t cur[CD_MAX];
    int size;
    int dirty_count;
} CellDiff;
void celldiff_init(CellDiff *c, int size) {
    if (!c) return;
    memset(c, 0, sizeof(*c));
    c->size = size > CD_MAX ? CD_MAX : size;
}
void celldiff_set(CellDiff *c, int idx, uint32_t cell) {
    if (!c || idx < 0 || idx >= c->size) return;
    c->cur[idx] = cell;
}
int celldiff_compute(CellDiff *c, int *dirty_indices, int cap) {
    if (!c) return -1;
    int n = 0;
    c->dirty_count = 0;
    for (int i = 0; i < c->size; i++) {
        if (c->cur[i] != c->prev[i]) {
            c->dirty_count++;
            if (dirty_indices && n < cap) dirty_indices[n++] = i;
        }
    }
    return n;
}
void celldiff_commit(CellDiff *c) {
    if (!c) return;
    memcpy(c->prev, c->cur, sizeof(uint32_t) * c->size);
}
int celldiff_dirty_count(const CellDiff *c) {
    return c ? c->dirty_count : -1;
}
