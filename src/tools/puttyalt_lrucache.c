/* puttyalt_lrucache.c - Fixed-size LRU cache (int keys).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define LRU_CAP 64
typedef struct { int key, val; long stamp; int used; } LruEnt;
typedef struct { LruEnt e[LRU_CAP]; long clock; } LruCache;
void lru_init(LruCache *c) { if (!c) return; c->clock = 0; for (int i=0;i<LRU_CAP;i++) c->e[i].used=0; }
int lru_get(LruCache *c, int key, int *val) {
    if (!c) return -1;
    for (int i=0;i<LRU_CAP;i++) if (c->e[i].used && c->e[i].key==key) {
        c->e[i].stamp = ++c->clock; if (val) *val = c->e[i].val; return 0;
    }
    return -1;
}
void lru_put(LruCache *c, int key, int val) {
    if (!c) return;
    for (int i=0;i<LRU_CAP;i++) if (c->e[i].used && c->e[i].key==key) { c->e[i].val=val; c->e[i].stamp=++c->clock; return; }
    int slot=-1; long oldest=0;
    for (int i=0;i<LRU_CAP;i++) { if (!c->e[i].used) { slot=i; break; } if (slot<0||c->e[i].stamp<oldest) { slot=i; oldest=c->e[i].stamp; } }
    c->e[slot].used=1; c->e[slot].key=key; c->e[slot].val=val; c->e[slot].stamp=++c->clock;
}
