/* puttyalt_smallmap.c - Small linear-probed string->int map. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define SMALLMAP_MAX 128
typedef struct {
    int keys[SMALLMAP_MAX];
    int vals[SMALLMAP_MAX];
    int len;
} smallmap_t;

int smallmap_reset(smallmap_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int smallmap_put(smallmap_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= SMALLMAP_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int smallmap_get(const smallmap_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int smallmap_len(const smallmap_t *m) { return m ? m->len : 0; }
