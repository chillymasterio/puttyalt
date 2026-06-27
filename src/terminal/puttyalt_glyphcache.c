/* puttyalt_glyphcache.c - LRU glyph rasterization cache.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define GC_CAP 64
typedef struct { unsigned int codepoint; int attr; long last_used; int valid; } GcEntry;
typedef struct { GcEntry entries[GC_CAP]; long clock; int hits; int misses; } GlyphCache;
void gc4_init(GlyphCache *c) { if (!c) return; c->clock = 0; c->hits = 0; c->misses = 0; for (int i=0;i<GC_CAP;i++) c->entries[i].valid = 0; }
/* Look up a glyph slot; returns index, or allocates one (LRU evict). */
int gc4_lookup(GlyphCache *c, unsigned int cp, int attr) {
    if (!c) return -1;
    for (int i = 0; i < GC_CAP; i++)
        if (c->entries[i].valid && c->entries[i].codepoint == cp && c->entries[i].attr == attr) {
            c->entries[i].last_used = ++c->clock; c->hits++; return i;
        }
    c->misses++;
    int slot = 0; long oldest = -1;
    for (int i = 0; i < GC_CAP; i++) {
        if (!c->entries[i].valid) { slot = i; break; }
        if (oldest < 0 || c->entries[i].last_used < oldest) { oldest = c->entries[i].last_used; slot = i; }
    }
    c->entries[slot].valid = 1; c->entries[slot].codepoint = cp;
    c->entries[slot].attr = attr; c->entries[slot].last_used = ++c->clock;
    return slot;
}
int gc4_hit_rate(const GlyphCache *c) {
    if (!c) return 0;
    int total = c->hits + c->misses;
    return total ? c->hits * 100 / total : 0;
}
