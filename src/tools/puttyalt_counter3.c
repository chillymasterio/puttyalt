/* puttyalt_counter3.c - Named counter registry with reset. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define COUNTER3_MAX 128
typedef struct {
    int keys[COUNTER3_MAX];
    int vals[COUNTER3_MAX];
    int len;
} counter3_t;

int counter3_reset(counter3_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int counter3_put(counter3_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= COUNTER3_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int counter3_get(const counter3_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int counter3_len(const counter3_t *m) { return m ? m->len : 0; }
