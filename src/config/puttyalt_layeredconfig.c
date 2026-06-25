/* puttyalt_layeredconfig.c - Resolve config across priority layers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define LC_LAYERS 4
#define LC_KEYS 32
typedef struct { char key[32]; char val[64]; int set; } LcKv;
typedef struct { LcKv layers[LC_LAYERS][LC_KEYS]; } LayeredConfig;
void lc2_init(LayeredConfig *c) {
    if (!c) return;
    for (int l = 0; l < LC_LAYERS; l++) for (int k = 0; k < LC_KEYS; k++) c->layers[l][k].set = 0;
}
int lc2_set(LayeredConfig *c, int layer, const char *key, const char *val) {
    if (!c || layer < 0 || layer >= LC_LAYERS || !key) return -1;
    for (int k = 0; k < LC_KEYS; k++) {
        LcKv *e = &c->layers[layer][k];
        if (e->set && strcmp(e->key, key) == 0) { strncpy(e->val, val, 63); e->val[63]=0; return 0; }
        if (!e->set) { strncpy(e->key, key, 31); e->key[31]=0; strncpy(e->val, val, 63); e->val[63]=0; e->set = 1; return 0; }
    }
    return -1;
}
/* Resolve: highest layer index wins. */
const char *lc2_get(const LayeredConfig *c, const char *key) {
    if (!c || !key) return 0;
    for (int l = LC_LAYERS - 1; l >= 0; l--)
        for (int k = 0; k < LC_KEYS; k++)
            if (c->layers[l][k].set && strcmp(c->layers[l][k].key, key) == 0) return c->layers[l][k].val;
    return 0;
}
