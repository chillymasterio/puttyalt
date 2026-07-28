/* puttyalt_runkeeper.c - Track longest run of equal values in a stream. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define RUNKEEPER_MAX 128
typedef struct {
    int keys[RUNKEEPER_MAX];
    int vals[RUNKEEPER_MAX];
    int len;
} runkeeper_t;

int runkeeper_reset(runkeeper_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int runkeeper_put(runkeeper_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= RUNKEEPER_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int runkeeper_get(const runkeeper_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int runkeeper_len(const runkeeper_t *m) { return m ? m->len : 0; }
