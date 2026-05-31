/* puttyalt_tofupin.c - Trust-on-first-use pin store with expiry for PuttyAlt. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TOFUPIN_MAX_ENTRIES   256
#define TOFUPIN_HOST_LEN      128
#define TOFUPIN_FP_LEN        128
#define TOFUPIN_RECORD_LEN    320

enum {
    TOFUPIN_OK = 0,
    TOFUPIN_ERR_NULL = -1,
    TOFUPIN_ERR_FULL = -2,
    TOFUPIN_ERR_RANGE = -3,
    TOFUPIN_ERR_NOTFOUND = -4
};

typedef enum {
    TOFUPIN_RESULT_NEW = 0,      /* first contact, newly pinned */
    TOFUPIN_RESULT_MATCH = 1,    /* presented fp matches active pin */
    TOFUPIN_RESULT_MISMATCH = 2, /* presented fp differs -> possible MITM */
    TOFUPIN_RESULT_EXPIRED = 3   /* pin expired, re-pinned on this use */
} tofupin_result_t;

typedef struct {
    char host[TOFUPIN_HOST_LEN];
    uint16_t port;
    char fingerprint[TOFUPIN_FP_LEN];
    uint64_t first_seen_ms;
    uint64_t expiry_ms;          /* 0 == never expires */
    int in_use;
} tofupin_entry_t;

typedef struct {
    tofupin_entry_t entries[TOFUPIN_MAX_ENTRIES];
    size_t count;
} tofupin_store_t;

int tofupin_init(tofupin_store_t *s)
{
    if (!s) return TOFUPIN_ERR_NULL;
    memset(s, 0, sizeof(*s));
    s->count = 0;
    return TOFUPIN_OK;
}

static tofupin_entry_t *tofupin_find(tofupin_store_t *s, const char *host, uint16_t port)
{
    size_t i;
    for (i = 0; i < TOFUPIN_MAX_ENTRIES; i++) {
        tofupin_entry_t *e = &s->entries[i];
        if (e->in_use && e->port == port && strncmp(e->host, host, TOFUPIN_HOST_LEN) == 0)
            return e;
    }
    return NULL;
}

int tofupin_is_expired(const tofupin_entry_t *e, uint64_t now_ms)
{
    if (!e || !e->in_use) return TOFUPIN_ERR_NULL;
    if (e->expiry_ms == 0) return 0;
    return (now_ms >= e->expiry_ms) ? 1 : 0;
}

int tofupin_pin(tofupin_store_t *s, const char *host, uint16_t port,
                const char *fingerprint, uint64_t now_ms, uint64_t ttl_ms)
{
    tofupin_entry_t *e;
    size_t i;
    if (!s || !host || !fingerprint) return TOFUPIN_ERR_NULL;

    e = tofupin_find(s, host, port);
    if (!e) {
        for (i = 0; i < TOFUPIN_MAX_ENTRIES; i++) {
            if (!s->entries[i].in_use) { e = &s->entries[i]; break; }
        }
        if (!e) return TOFUPIN_ERR_FULL;
        memset(e, 0, sizeof(*e));
        e->in_use = 1;
        s->count++;
    }
    strncpy(e->host, host, TOFUPIN_HOST_LEN - 1);
    e->host[TOFUPIN_HOST_LEN - 1] = '\0';
    strncpy(e->fingerprint, fingerprint, TOFUPIN_FP_LEN - 1);
    e->fingerprint[TOFUPIN_FP_LEN - 1] = '\0';
    e->port = port;
    e->first_seen_ms = now_ms;
    e->expiry_ms = (ttl_ms == 0) ? 0 : now_ms + ttl_ms;
    return TOFUPIN_OK;
}

const tofupin_entry_t *tofupin_lookup(const tofupin_store_t *s, const char *host, uint16_t port)
{
    if (!s || !host) return NULL;
    return tofupin_find((tofupin_store_t *)s, host, port);
}

tofupin_result_t tofupin_verify(tofupin_store_t *s, const char *host, uint16_t port,
                                const char *fingerprint, uint64_t now_ms, uint64_t ttl_ms)
{
    tofupin_entry_t *e;
    if (!s || !host || !fingerprint) return TOFUPIN_RESULT_MISMATCH;

    e = tofupin_find(s, host, port);
    if (!e) {
        tofupin_pin(s, host, port, fingerprint, now_ms, ttl_ms);
        return TOFUPIN_RESULT_NEW;
    }
    if (tofupin_is_expired(e, now_ms) == 1) {
        tofupin_pin(s, host, port, fingerprint, now_ms, ttl_ms);
        return TOFUPIN_RESULT_EXPIRED;
    }
    if (strncmp(e->fingerprint, fingerprint, TOFUPIN_FP_LEN) == 0)
        return TOFUPIN_RESULT_MATCH;
    return TOFUPIN_RESULT_MISMATCH;
}

int tofupin_revoke(tofupin_store_t *s, const char *host, uint16_t port)
{
    tofupin_entry_t *e;
    if (!s || !host) return TOFUPIN_ERR_NULL;
    e = tofupin_find(s, host, port);
    if (!e) return TOFUPIN_ERR_NOTFOUND;
    memset(e, 0, sizeof(*e));
    if (s->count > 0) s->count--;
    return TOFUPIN_OK;
}

int tofupin_prune_expired(tofupin_store_t *s, uint64_t now_ms)
{
    size_t i;
    int pruned = 0;
    if (!s) return TOFUPIN_ERR_NULL;
    for (i = 0; i < TOFUPIN_MAX_ENTRIES; i++) {
        tofupin_entry_t *e = &s->entries[i];
        if (e->in_use && tofupin_is_expired(e, now_ms) == 1) {
            memset(e, 0, sizeof(*e));
            if (s->count > 0) s->count--;
            pruned++;
        }
    }
    return pruned;
}

int tofupin_export_record(const tofupin_entry_t *e, char *buf, size_t buflen)
{
    int n;
    if (!e || !buf) return TOFUPIN_ERR_NULL;
    if (buflen == 0 || buflen > TOFUPIN_RECORD_LEN) return TOFUPIN_ERR_RANGE;
    n = snprintf(buf, buflen, "%s:%u %s first=%llu expiry=%llu",
                 e->host, (unsigned)e->port, e->fingerprint,
                 (unsigned long long)e->first_seen_ms,
                 (unsigned long long)e->expiry_ms);
    if (n < 0 || (size_t)n >= buflen) return TOFUPIN_ERR_RANGE;
    return n;
}

size_t tofupin_count(const tofupin_store_t *s)
{
    return s ? s->count : 0;
}

const char *tofupin_result_name(tofupin_result_t r)
{
    switch (r) {
        case TOFUPIN_RESULT_NEW:      return "NEW";
        case TOFUPIN_RESULT_MATCH:    return "MATCH";
        case TOFUPIN_RESULT_MISMATCH: return "MISMATCH";
        case TOFUPIN_RESULT_EXPIRED:  return "EXPIRED";
        default:                      return "UNKNOWN";
    }
}

const char *tofupin_strerror(int code)
{
    switch (code) {
        case TOFUPIN_OK:            return "ok";
        case TOFUPIN_ERR_NULL:      return "null argument";
        case TOFUPIN_ERR_FULL:      return "pin store full";
        case TOFUPIN_ERR_RANGE:     return "buffer range error";
        case TOFUPIN_ERR_NOTFOUND:  return "pin not found";
        default:                    return "unknown error";
    }
}
