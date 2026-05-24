#ifndef PUTTYALT_RATELIMIT_H
#define PUTTYALT_RATELIMIT_H

#define RL_MAX_BUCKETS  32
#define RL_MAX_NAME     64

typedef struct {
    char   name[RL_MAX_NAME];
    int    max_tokens;
    int    tokens;
    int    refill_rate;     /* tokens per second */
    long   last_refill;
    long   total_allowed;
    long   total_denied;
} RLBucket;

typedef struct {
    RLBucket buckets[RL_MAX_BUCKETS];
    int      count;
    int      global_enabled;
} RateLimiter;

void rl_init(RateLimiter *rl);
int  rl_add_bucket(RateLimiter *rl, const char *name, int max_tokens, int refill_rate);
int  rl_try_acquire(RateLimiter *rl, const char *name, int tokens);
int  rl_find(const RateLimiter *rl, const char *name);
void rl_refill(RateLimiter *rl);
void rl_reset(RateLimiter *rl, const char *name);
void rl_enable(RateLimiter *rl, int enable);

#endif
