/* puttyalt_medianwin.c - Sliding-window median over integers. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define MEDIANWIN_MAX 128
typedef struct {
    int keys[MEDIANWIN_MAX];
    int vals[MEDIANWIN_MAX];
    int len;
} medianwin_t;

int medianwin_reset(medianwin_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int medianwin_put(medianwin_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= MEDIANWIN_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int medianwin_get(const medianwin_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int medianwin_len(const medianwin_t *m) { return m ? m->len : 0; }
