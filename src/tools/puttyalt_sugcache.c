/* puttyalt_sugcache.c - suggestion result cache with TTL and LRU eviction. */
#include <string.h>
#include <stdio.h>

#define SUGCACHE_SLOTS   64
#define SUGCACHE_KEYLEN  128
#define SUGCACHE_VALLEN  1024

typedef struct {
    char          key[SUGCACHE_KEYLEN];
    char          value[SUGCACHE_VALLEN];
    unsigned long expires;   /* now_ms at which entry becomes stale */
    unsigned long used;      /* monotonic counter for LRU ordering   */
    int           in_use;
} SugCacheEntry;

typedef struct {
    SugCacheEntry entries[SUGCACHE_SLOTS];
    unsigned long ttl_ms;
    unsigned long tick;      /* incremented on every access           */
    unsigned long hits;
    unsigned long misses;
    unsigned long evictions;
} SugCache;

typedef struct {
    unsigned long entries;
    unsigned long hits;
    unsigned long misses;
    unsigned long evictions;
} SugCacheStats;

void sugcache_init(SugCache *c, unsigned long ttl_ms)
{
    if (!c) return;
    memset(c, 0, sizeof(*c));
    c->ttl_ms = ttl_ms;
}

static int sugcache_find(const SugCache *c, const char *key)
{
    int i;
    for (i = 0; i < SUGCACHE_SLOTS; i++) {
        if (c->entries[i].in_use &&
            strncmp(c->entries[i].key, key, SUGCACHE_KEYLEN - 1) == 0)
            return i;
    }
    return -1;
}

void sugcache_evict_expired(SugCache *c, unsigned long now)
{
    int i;
    if (!c) return;
    for (i = 0; i < SUGCACHE_SLOTS; i++) {
        if (c->entries[i].in_use && now >= c->entries[i].expires) {
            c->entries[i].in_use = 0;
            c->evictions++;
        }
    }
}

int sugcache_get(SugCache *c, const char *key, char *out, size_t outcap,
                 unsigned long now)
{
    int idx;
    if (!c || !key || !out || outcap == 0) return -1;
    idx = sugcache_find(c, key);
    if (idx < 0) { c->misses++; return -1; }
    if (now >= c->entries[idx].expires) {
        c->entries[idx].in_use = 0;
        c->evictions++;
        c->misses++;
        return -1;
    }
    c->entries[idx].used = ++c->tick;
    c->hits++;
    snprintf(out, outcap, "%s", c->entries[idx].value);
    return 0;
}

void sugcache_put(SugCache *c, const char *key, const char *value,
                  unsigned long now)
{
    int i, idx;
    unsigned long oldest;
    if (!c || !key || !value) return;
    if (key[0] == '\0') return;

    idx = sugcache_find(c, key);
    if (idx < 0) {
        idx = -1;
        for (i = 0; i < SUGCACHE_SLOTS; i++) {
            if (!c->entries[i].in_use) { idx = i; break; }
        }
        if (idx < 0) {
            oldest = 0;
            idx = 0;
            for (i = 0; i < SUGCACHE_SLOTS; i++) {
                if (i == 0 || c->entries[i].used < oldest) {
                    oldest = c->entries[i].used;
                    idx = i;
                }
            }
            c->evictions++;
        }
    }
    snprintf(c->entries[idx].key, SUGCACHE_KEYLEN, "%s", key);
    snprintf(c->entries[idx].value, SUGCACHE_VALLEN, "%s", value);
    c->entries[idx].expires = now + c->ttl_ms;
    c->entries[idx].used = ++c->tick;
    c->entries[idx].in_use = 1;
}

void sugcache_invalidate(SugCache *c, const char *key)
{
    int idx;
    if (!c || !key) return;
    idx = sugcache_find(c, key);
    if (idx >= 0)
        c->entries[idx].in_use = 0;
}

int sugcache_stats(const SugCache *c, SugCacheStats *out)
{
    int i;
    if (!c || !out) return -1;
    out->entries = 0;
    out->hits = c->hits;
    out->misses = c->misses;
    out->evictions = c->evictions;
    for (i = 0; i < SUGCACHE_SLOTS; i++) {
        if (c->entries[i].in_use)
            out->entries++;
    }
    return 0;
}
