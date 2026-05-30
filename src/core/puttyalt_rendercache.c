/*
 * puttyalt_rendercache.c - Render cache for PuttyAlt.
 *
 * Caches rendered glyph-run keys (codepoint, fg/bg, attr) to reusable slot
 * indices using a fixed-size table with LRU eviction, avoiding the cost of
 * re-rasterizing identical cells.
 *
 * Self-contained C99 library module. No project headers, no windows.h.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define RENDERCACHE_SLOTS 256

typedef struct RenderCacheEntry {
    unsigned int  key;   /* codepoint or glyph-run key */
    unsigned int  fg;    /* foreground colour          */
    unsigned int  bg;    /* background colour          */
    unsigned char attr;  /* attribute bits             */
    unsigned char used;  /* 1 if slot is occupied      */
    uint64_t      lru;   /* monotonic last-use counter */
} RenderCacheEntry;

typedef struct RenderCache {
    RenderCacheEntry slots[RENDERCACHE_SLOTS];
    uint64_t         tick;    /* monotonic use counter    */
    int              count;   /* number of occupied slots */
    int              hits;
    int              misses;
} RenderCache;

void rendercache_init(RenderCache *c)
{
    if (!c)
        return;
    memset(c, 0, sizeof(*c));
}

/* Returns slot index [0,RENDERCACHE_SLOTS) on hit, or -1 on miss/error. */
int rendercache_lookup(RenderCache *c, unsigned int key, unsigned int fg,
                       unsigned int bg, unsigned char attr)
{
    int i;
    if (!c)
        return -1;
    for (i = 0; i < RENDERCACHE_SLOTS; i++) {
        RenderCacheEntry *e = &c->slots[i];
        if (e->used && e->key == key && e->fg == fg &&
            e->bg == bg && e->attr == attr) {
            e->lru = ++c->tick;
            c->hits++;
            return i;
        }
    }
    c->misses++;
    return -1;
}

/* Inserts (or refreshes) an entry, evicting the LRU slot if full.
 * Returns the slot index on success, or -1 on error. */
int rendercache_insert(RenderCache *c, unsigned int key, unsigned int fg,
                       unsigned int bg, unsigned char attr)
{
    int i, victim = 0;
    uint64_t oldest;
    RenderCacheEntry *e;
    if (!c)
        return -1;

    for (i = 0; i < RENDERCACHE_SLOTS; i++) {     /* refresh if present */
        e = &c->slots[i];
        if (e->used && e->key == key && e->fg == fg &&
            e->bg == bg && e->attr == attr) {
            e->lru = ++c->tick;
            return i;
        }
    }

    for (i = 0; i < RENDERCACHE_SLOTS; i++) {     /* prefer empty slot */
        if (!c->slots[i].used) {
            victim = i;
            goto fill;
        }
    }

    oldest = c->slots[0].lru;                     /* else evict LRU */
    for (i = 1; i < RENDERCACHE_SLOTS; i++) {
        if (c->slots[i].lru < oldest) {
            oldest = c->slots[i].lru;
            victim = i;
        }
    }
    c->count--;

fill:
    e = &c->slots[victim];
    e->key  = key;
    e->fg   = fg;
    e->bg   = bg;
    e->attr = attr;
    e->used = 1;
    e->lru  = ++c->tick;
    c->count++;
    return victim;
}

void rendercache_invalidate(RenderCache *c)
{
    int saved_hits, saved_misses;
    if (!c)
        return;
    saved_hits = c->hits;
    saved_misses = c->misses;
    memset(c, 0, sizeof(*c));
    c->hits = saved_hits;
    c->misses = saved_misses;
}

/* Writes hit/miss counters through the out-pointers (if non-NULL).
 * Returns 0 on success, -1 on error. */
int rendercache_stats(const RenderCache *c, int *hits, int *misses)
{
    char buf[64];
    if (!c)
        return -1;
    if (hits)
        *hits = c->hits;
    if (misses)
        *misses = c->misses;
    (void)snprintf(buf, sizeof(buf), "rendercache: %d/%d used, h=%d m=%d",
                   c->count, RENDERCACHE_SLOTS, c->hits, c->misses);
    return 0;
}
