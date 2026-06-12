/* puttyalt_retrytimer.c - Retry timer with configurable backoff strategies. */
#include <string.h>
#include <stdint.h>
enum rt_strategy { RT_FIXED = 0, RT_LINEAR = 1, RT_EXPONENTIAL = 2, RT_FIBONACCI = 3 };
typedef struct {
    int strategy;
    int base_ms;
    int max_ms;
    int attempt;
    int fib_a, fib_b;
} RetryTimer;
void retrytimer_init(RetryTimer *r, int strategy, int base_ms, int max_ms) {
    if (!r) return;
    memset(r, 0, sizeof(*r));
    r->strategy = strategy;
    r->base_ms = base_ms > 0 ? base_ms : 1000;
    r->max_ms = max_ms > 0 ? max_ms : 60000;
    r->fib_a = 1;
    r->fib_b = 1;
}
int retrytimer_next(RetryTimer *r) {
    if (!r) return -1;
    int delay;
    switch (r->strategy) {
        case RT_LINEAR:
            delay = r->base_ms * (r->attempt + 1);
            break;
        case RT_EXPONENTIAL:
            delay = r->base_ms;
            for (int i = 0; i < r->attempt && delay < r->max_ms; i++) delay *= 2;
            break;
        case RT_FIBONACCI: {
            delay = r->base_ms * r->fib_a;
            int next = r->fib_a + r->fib_b;
            r->fib_a = r->fib_b;
            r->fib_b = next;
            break;
        }
        default:
            delay = r->base_ms;
            break;
    }
    if (delay > r->max_ms) delay = r->max_ms;
    r->attempt++;
    return delay;
}
void retrytimer_reset(RetryTimer *r) {
    if (!r) return;
    r->attempt = 0;
    r->fib_a = 1;
    r->fib_b = 1;
}
int retrytimer_attempt(const RetryTimer *r) {
    return r ? r->attempt : -1;
}
