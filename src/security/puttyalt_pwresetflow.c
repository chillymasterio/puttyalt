/* puttyalt_pwresetflow.c - State machine for a password reset flow. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define PWRESETFLOW_MAX 128
typedef struct {
    int keys[PWRESETFLOW_MAX];
    int vals[PWRESETFLOW_MAX];
    int len;
} pwresetflow_t;

int pwresetflow_reset(pwresetflow_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int pwresetflow_put(pwresetflow_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= PWRESETFLOW_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int pwresetflow_get(const pwresetflow_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int pwresetflow_len(const pwresetflow_t *m) { return m ? m->len : 0; }
