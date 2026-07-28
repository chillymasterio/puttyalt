/* puttyalt_bitset2.c - Fixed-capacity bit set with popcount. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define BITSET2_MAX 128
typedef struct {
    int keys[BITSET2_MAX];
    int vals[BITSET2_MAX];
    int len;
} bitset2_t;

int bitset2_reset(bitset2_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int bitset2_put(bitset2_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= BITSET2_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int bitset2_get(const bitset2_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int bitset2_len(const bitset2_t *m) { return m ? m->len : 0; }
