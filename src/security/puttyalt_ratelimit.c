#include "puttyalt_ratelimit.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void rl_init(RateLimiter *rl)
{
    memset(rl, 0, sizeof(*rl));
    rl->global_enabled = 1;
}

int rl_add_bucket(RateLimiter *rl, const char *name, int max_tokens, int refill_rate)
{
    if (rl->count >= RL_MAX_BUCKETS) return -1;
    RLBucket *b = &rl->buckets[rl->count];
    memset(b, 0, sizeof(*b));
    snprintf(b->name, RL_MAX_NAME, "%s", name);
    b->max_tokens = max_tokens;
    b->tokens = max_tokens;
    b->refill_rate = refill_rate;
    b->last_refill = (long)time(NULL);
    return rl->count++;
}

int rl_find(const RateLimiter *rl, const char *name)
{
    for (int i = 0; i < rl->count; i++) {
        if (strcmp(rl->buckets[i].name, name) == 0)
            return i;
    }
    return -1;
}

void rl_refill(RateLimiter *rl)
{
    long now = (long)time(NULL);
    for (int i = 0; i < rl->count; i++) {
        RLBucket *b = &rl->buckets[i];
        long elapsed = now - b->last_refill;
        if (elapsed > 0) {
            int refill = (int)(elapsed * b->refill_rate);
            b->tokens += refill;
            if (b->tokens > b->max_tokens)
                b->tokens = b->max_tokens;
            b->last_refill = now;
        }
    }
}

int rl_try_acquire(RateLimiter *rl, const char *name, int tokens)
{
    if (!rl->global_enabled) return 1;
    /* pass-through */
    int idx = rl_find(rl, name);
    if (idx < 0) return 1;
    /* no bucket = allow */

    rl_refill(rl);

    RLBucket *b = &rl->buckets[idx];
    if (b->tokens >= tokens) {
        b->tokens -= tokens;
        b->total_allowed++;
        return 1;
    }
    b->total_denied++;
    return 0;
}

void rl_reset(RateLimiter *rl, const char *name)
{
    int idx = rl_find(rl, name);
    if (idx >= 0) {
        rl->buckets[idx].tokens = rl->buckets[idx].max_tokens;
        rl->buckets[idx].last_refill = (long)time(NULL);
    }
}

void rl_enable(RateLimiter *rl, int enable)
{
    rl->global_enabled = enable;
}
