/* puttyalt_keyrotate.c - Key rotation schedule for PuttyAlt.
 * Pure logic: interval and usage-count policies, grace windows,
 * due/overdue detection against a supplied clock. C99, std headers only. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define KEYROTATE_MAX_HISTORY 16

enum keyrotate_policy {
    KEYROTATE_POLICY_NONE = 0,
    KEYROTATE_POLICY_INTERVAL,   /* rotate every interval_ms */
    KEYROTATE_POLICY_USAGE,      /* rotate after usage_limit uses */
    KEYROTATE_POLICY_BOTH        /* rotate when either trips */
};

enum keyrotate_err {
    KEYROTATE_OK = 0,
    KEYROTATE_ERR_NULL = -1,
    KEYROTATE_ERR_RANGE = -2,
    KEYROTATE_ERR_POLICY = -3
};

typedef struct keyrotate_state {
    int policy;                  /* enum keyrotate_policy */
    uint64_t interval_ms;        /* rotation interval in ms */
    uint64_t grace_ms;           /* overlap window after due */
    uint64_t usage_limit;        /* max uses before rotation */
    uint64_t usage_count;        /* uses since last rotation */
    uint64_t last_rotated_ms;    /* timestamp of last rotation */
    uint32_t rotations;          /* total completed rotations */
    uint64_t history[KEYROTATE_MAX_HISTORY]; /* ring of rotation times */
    int hist_head;               /* next write slot in history */
} keyrotate_state;

int keyrotate_init(keyrotate_state *st, uint64_t now_ms) {
    if (!st) return KEYROTATE_ERR_NULL;
    memset(st, 0, sizeof(*st));
    st->policy = KEYROTATE_POLICY_NONE;
    st->last_rotated_ms = now_ms;
    return KEYROTATE_OK;
}

int keyrotate_set_policy(keyrotate_state *st, int policy,
                         uint64_t interval_ms, uint64_t usage_limit,
                         uint64_t grace_ms) {
    if (!st) return KEYROTATE_ERR_NULL;
    if (policy < KEYROTATE_POLICY_NONE || policy > KEYROTATE_POLICY_BOTH)
        return KEYROTATE_ERR_POLICY;
    if ((policy == KEYROTATE_POLICY_INTERVAL || policy == KEYROTATE_POLICY_BOTH)
        && interval_ms == 0)
        return KEYROTATE_ERR_RANGE;
    if ((policy == KEYROTATE_POLICY_USAGE || policy == KEYROTATE_POLICY_BOTH)
        && usage_limit == 0)
        return KEYROTATE_ERR_RANGE;
    st->policy = policy;
    st->interval_ms = interval_ms;
    st->usage_limit = usage_limit;
    st->grace_ms = grace_ms;
    return KEYROTATE_OK;
}

int keyrotate_record_use(keyrotate_state *st, uint64_t count) {
    if (!st) return KEYROTATE_ERR_NULL;
    if (st->usage_count > UINT64_MAX - count) st->usage_count = UINT64_MAX;
    else st->usage_count += count;
    return KEYROTATE_OK;
}

static int keyrotate_interval_due(const keyrotate_state *st, uint64_t now_ms) {
    return now_ms >= st->last_rotated_ms &&
           (now_ms - st->last_rotated_ms) >= st->interval_ms;
}

int keyrotate_is_due(const keyrotate_state *st, uint64_t now_ms) {
    int iv, us;
    if (!st) return KEYROTATE_ERR_NULL;
    iv = (st->policy == KEYROTATE_POLICY_INTERVAL ||
          st->policy == KEYROTATE_POLICY_BOTH) && keyrotate_interval_due(st, now_ms);
    us = (st->policy == KEYROTATE_POLICY_USAGE ||
          st->policy == KEYROTATE_POLICY_BOTH) && st->usage_count >= st->usage_limit;
    if (st->policy == KEYROTATE_POLICY_NONE) return 0;
    return (iv || us) ? 1 : 0;
}

uint64_t keyrotate_next_at(const keyrotate_state *st) {
    if (!st || (st->policy != KEYROTATE_POLICY_INTERVAL &&
                st->policy != KEYROTATE_POLICY_BOTH))
        return 0;
    return st->last_rotated_ms + st->interval_ms;
}

int64_t keyrotate_seconds_until_due(const keyrotate_state *st, uint64_t now_ms) {
    uint64_t next;
    if (!st) return KEYROTATE_ERR_NULL;
    next = keyrotate_next_at(st);
    if (next == 0) return -1;
    if (now_ms >= next) return 0;
    return (int64_t)((next - now_ms) / 1000u);
}

int keyrotate_mark_rotated(keyrotate_state *st, uint64_t now_ms) {
    if (!st) return KEYROTATE_ERR_NULL;
    st->last_rotated_ms = now_ms;
    st->usage_count = 0;
    if (st->rotations < UINT32_MAX) st->rotations++;
    st->history[st->hist_head] = now_ms;
    st->hist_head = (st->hist_head + 1) % KEYROTATE_MAX_HISTORY;
    return KEYROTATE_OK;
}

int keyrotate_in_grace_window(const keyrotate_state *st, uint64_t now_ms) {
    uint64_t next;
    if (!st) return KEYROTATE_ERR_NULL;
    if (!keyrotate_is_due(st, now_ms)) return 0;
    next = keyrotate_next_at(st);
    if (next == 0) return st->grace_ms > 0 ? 1 : 0;
    if (now_ms < next) return 0;
    return (now_ms - next) <= st->grace_ms ? 1 : 0;
}

const char *keyrotate_policy_name(int policy) {
    switch (policy) {
        case KEYROTATE_POLICY_NONE:     return "none";
        case KEYROTATE_POLICY_INTERVAL: return "interval";
        case KEYROTATE_POLICY_USAGE:    return "usage";
        case KEYROTATE_POLICY_BOTH:     return "both";
        default:                        return "unknown";
    }
}

const char *keyrotate_strerror(int err) {
    switch (err) {
        case KEYROTATE_OK:          return "ok";
        case KEYROTATE_ERR_NULL:    return "null argument";
        case KEYROTATE_ERR_RANGE:   return "value out of range";
        case KEYROTATE_ERR_POLICY:  return "invalid policy";
        default:                    return "unknown error";
    }
}

int keyrotate_format_schedule(const keyrotate_state *st, uint64_t now_ms,
                              char *buf, size_t buflen) {
    int n;
    int64_t secs;
    if (!st || !buf || buflen == 0) return KEYROTATE_ERR_NULL;
    secs = keyrotate_seconds_until_due(st, now_ms);
    n = snprintf(buf, buflen,
                 "policy=%s next_at=%llu due_in=%llds uses=%llu/%llu rot=%u %s",
                 keyrotate_policy_name(st->policy),
                 (unsigned long long)keyrotate_next_at(st),
                 (long long)(secs < 0 ? 0 : secs),
                 (unsigned long long)st->usage_count,
                 (unsigned long long)st->usage_limit,
                 (unsigned)st->rotations,
                 keyrotate_is_due(st, now_ms) ? "DUE" : "ok");
    if (n < 0 || (size_t)n >= buflen) return KEYROTATE_ERR_RANGE;
    return n;
}
