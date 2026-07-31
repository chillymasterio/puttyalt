/* puttyalt_yamlmap.c - Parse a flat YAML mapping into key/value pairs. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define YAMLMAP_MAX 128
typedef struct {
    int keys[YAMLMAP_MAX];
    int vals[YAMLMAP_MAX];
    int len;
} yamlmap_t;

int yamlmap_reset(yamlmap_t *m) {
    if (!m) return -1;
    m->len = 0;
    return 0;
}
int yamlmap_put(yamlmap_t *m, int k, int v) {
    if (!m) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { m->vals[i] = v; return 0; }
    }
    if (m->len >= YAMLMAP_MAX) return -1;
    m->keys[m->len] = k;
    m->vals[m->len] = v;
    m->len++;
    return 0;
}
int yamlmap_get(const yamlmap_t *m, int k, int *out) {
    if (!m || !out) return -1;
    for (int i = 0; i < m->len; i++) {
        if (m->keys[i] == k) { *out = m->vals[i]; return 0; }
    }
    return -1;
}
int yamlmap_len(const yamlmap_t *m) { return m ? m->len : 0; }
