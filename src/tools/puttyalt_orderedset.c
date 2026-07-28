/* puttyalt_orderedset.c - Insertion-order preserving string set. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define ORDEREDSET_MAX 128
typedef struct {
    int keys[ORDEREDSET_MAX];
    int vals[ORDEREDSET_MAX];
    int len;
} orderedset_t;

int orderedset_reset(orderedset_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int orderedset_put(orderedset_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= ORDEREDSET_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int orderedset_get(const orderedset_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int orderedset_len(const orderedset_t *m) { return m ? m->len : 0; }
