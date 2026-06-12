/* puttyalt_cache2.c - LRU cache v2 with TTL + size eviction. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CA_MAX 64
#define CA_KEY 48
#define CA_VAL 128
typedef struct {
    char key[CA_KEY];
    char value[CA_VAL];
    uint64_t last_access;
    uint64_t expires;
    int valid;
} ca_entry;
typedef struct {
    ca_entry entries[CA_MAX];
    int n;
    int hits, misses;
    int ttl_ms;
} Cache2;
void cache2_init(Cache2 *c, int ttl_ms) {
    if (!c) return;
    memset(c, 0, sizeof(*c));
    c->ttl_ms = ttl_ms > 0 ? ttl_ms : 60000;
}
int cache2_put(Cache2 *c, const char *key, const char *value, uint64_t now_ms) {
    if (!c || !key) return -1;
    for (int i = 0; i < c->n; i++) {
        if (c->entries[i].valid && strcmp(c->entries[i].key, key) == 0) {
            snprintf(c->entries[i].value, CA_VAL, "%s", value ? value : "");
            c->entries[i].last_access = now_ms;
            c->entries[i].expires = now_ms + c->ttl_ms;
            return 0;
        }
    }
    ca_entry *e = NULL;
    if (c->n < CA_MAX) {
        e = &c->entries[c->n++];
    } else {
        int lru = 0;
        for (int i = 1; i < c->n; i++)
            if (c->entries[i].last_access < c->entries[lru].last_access) lru = i;
        e = &c->entries[lru];
    }
    snprintf(e->key, CA_KEY, "%s", key);
    snprintf(e->value, CA_VAL, "%s", value ? value : "");
    e->last_access = now_ms;
    e->expires = now_ms + c->ttl_ms;
    e->valid = 1;
    return 0;
}
int cache2_get(Cache2 *c, const char *key, uint64_t now_ms, char *out, int outlen) {
    if (!c || !key) return -1;
    for (int i = 0; i < c->n; i++) {
        if (c->entries[i].valid && strcmp(c->entries[i].key, key) == 0) {
            if (now_ms >= c->entries[i].expires) {
                c->entries[i].valid = 0;
                c->misses++;
                return -1;
            }
            c->entries[i].last_access = now_ms;
            c->hits++;
            if (out) snprintf(out, outlen, "%s", c->entries[i].value);
            return 0;
        }
    }
    c->misses++;
    return -1;
}
int cache2_hit_rate(const Cache2 *c) {
    if (!c) return -1;
    int total = c->hits + c->misses;
    return total ? c->hits * 100 / total : 0;
}
int cache2_count(const Cache2 *c) {
    if (!c) return -1;
    int n = 0;
    for (int i = 0; i < c->n; i++)
        if (c->entries[i].valid) n++;
    return n;
}
