/* puttyalt_topN.c - Maintain the top-N largest floats seen. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define TOPN_MAX 128
typedef struct {
    int keys[TOPN_MAX];
    int vals[TOPN_MAX];
    int len;
} topN_t;

int topN_reset(topN_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int topN_put(topN_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= TOPN_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int topN_get(const topN_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int topN_len(const topN_t *m) { return m ? m->len : 0; }
