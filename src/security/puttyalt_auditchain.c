/* PuttyAlt: tamper-evident hash-chained audit log (self-contained, C99). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define AUDITCHAIN_CAP     256
#define AUDITCHAIN_PAYLEN  96
#define AUDITCHAIN_GENESIS 0x1505u

enum { AUDITCHAIN_OK = 0, AUDITCHAIN_ERR_ARG = 1, AUDITCHAIN_ERR_FULL = 2,
       AUDITCHAIN_ERR_RANGE = 3, AUDITCHAIN_ERR_BREAK = 4 };

typedef struct {
    uint32_t seq;          /* monotonic sequence number */
    uint64_t now_ms;       /* caller-supplied timestamp */
    uint32_t payload_hash; /* digest of the payload bytes */
    uint32_t prev_hash;    /* hash of previous entry (genesis for first) */
    uint32_t chain_hash;   /* hash linking this entry to the chain */
    char     payload[AUDITCHAIN_PAYLEN];
} auditchain_entry_t;

typedef struct {
    auditchain_entry_t ring[AUDITCHAIN_CAP];
    uint32_t count, next_seq, head_hash;
} auditchain_t;

/* Lightweight FNV-1a style digest over a byte buffer with a seed. */
uint32_t auditchain_digest(uint32_t seed, const void *data, size_t len)
{
    const unsigned char *p = (const unsigned char *)data;
    uint32_t h = seed ? seed : AUDITCHAIN_GENESIS;
    size_t i;
    for (i = 0; i < len; i++) { h ^= (uint32_t)p[i]; h *= 16777619u; }
    return h;
}

int auditchain_init(auditchain_t *c)
{
    if (!c) return -1;
    memset(c, 0, sizeof(*c));
    c->head_hash = AUDITCHAIN_GENESIS;
    return 0;
}

int auditchain_reset(auditchain_t *c) { return auditchain_init(c); }

uint32_t auditchain_count(const auditchain_t *c) { return c ? c->count : 0; }

uint32_t auditchain_head_hash(const auditchain_t *c)
{
    return c ? c->head_hash : AUDITCHAIN_GENESIS;
}

const auditchain_entry_t *auditchain_entry_at(const auditchain_t *c, uint32_t idx)
{
    if (!c || idx >= c->count) return NULL;
    return &c->ring[idx];
}

/* Recompute the expected chain hash for one entry given the prior hash. */
static uint32_t auditchain_expect(const auditchain_entry_t *e, uint32_t prev)
{
    uint32_t mix = prev ^ (e->seq * 2654435761u) ^ e->payload_hash;
    return auditchain_digest(mix, &e->now_ms, sizeof(e->now_ms));
}

int auditchain_append(auditchain_t *c, const char *payload, uint64_t now_ms)
{
    auditchain_entry_t *e;
    if (!c || !payload) return -1;
    if (c->count >= AUDITCHAIN_CAP) return -1;
    e = &c->ring[c->count];
    memset(e, 0, sizeof(*e));
    e->seq = c->next_seq;
    e->now_ms = now_ms;
    strncpy(e->payload, payload, AUDITCHAIN_PAYLEN - 1);
    e->payload[AUDITCHAIN_PAYLEN - 1] = '\0';
    e->payload_hash = auditchain_digest(AUDITCHAIN_GENESIS, e->payload,
                                        strlen(e->payload));
    e->prev_hash = c->head_hash;
    e->chain_hash = auditchain_expect(e, e->prev_hash);
    c->head_hash = e->chain_hash;
    c->count++;
    c->next_seq++;
    return 0;
}

/* Returns index of first broken entry, or -1 if the chain is intact. */
int auditchain_find_break(const auditchain_t *c)
{
    uint32_t i, prev = AUDITCHAIN_GENESIS;
    if (!c) return -1;
    for (i = 0; i < c->count; i++) {
        const auditchain_entry_t *e = &c->ring[i];
        uint32_t ph = auditchain_digest(AUDITCHAIN_GENESIS, e->payload,
                                        strlen(e->payload));
        if (e->seq != i || e->prev_hash != prev || e->payload_hash != ph ||
            e->chain_hash != auditchain_expect(e, prev))
            return (int)i;
        prev = e->chain_hash;
    }
    return -1;
}

int auditchain_verify(const auditchain_t *c)
{
    if (!c || auditchain_find_break(c) >= 0) return -1;
    if (c->count > 0 && c->head_hash != c->ring[c->count - 1].chain_hash)
        return -1;
    return 0;
}

int auditchain_serialize_entry(const auditchain_t *c, uint32_t idx,
                               char *buf, size_t buflen)
{
    const auditchain_entry_t *e = auditchain_entry_at(c, idx);
    int n;
    if (!buf || buflen == 0 || !e) return -1;
    n = snprintf(buf, buflen,
                 "seq=%lu ts=%llu pay=%08lx prev=%08lx chain=%08lx msg=%s",
                 (unsigned long)e->seq, (unsigned long long)e->now_ms,
                 (unsigned long)e->payload_hash, (unsigned long)e->prev_hash,
                 (unsigned long)e->chain_hash, e->payload);
    if (n < 0 || (size_t)n >= buflen) return -1;
    return n;
}

const char *auditchain_strerror(int code)
{
    switch (code) {
        case AUDITCHAIN_OK:        return "ok";
        case AUDITCHAIN_ERR_ARG:   return "invalid argument";
        case AUDITCHAIN_ERR_FULL:  return "chain full";
        case AUDITCHAIN_ERR_RANGE: return "index out of range";
        case AUDITCHAIN_ERR_BREAK: return "chain integrity broken";
        default:                   return "unknown error";
    }
}
