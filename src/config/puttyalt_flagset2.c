/* puttyalt_flagset2.c - Parse space or comma separated flag names. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define FLAGSET2_MAX 128
typedef struct {
    int keys[FLAGSET2_MAX];
    int vals[FLAGSET2_MAX];
    int len;
} flagset2_t;

int flagset2_reset(flagset2_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int flagset2_put(flagset2_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= FLAGSET2_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int flagset2_get(const flagset2_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int flagset2_len(const flagset2_t *m) { return m ? m->len : 0; }
