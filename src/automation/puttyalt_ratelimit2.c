/* puttyalt_ratelimit2.c - Token-bucket rate limiter.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { double tokens; double rate; double cap; long last; } RateBucket;
void rl2_init(RateBucket *b, double rate, double cap) {
    if (!b) return;
    b->rate = rate > 0 ? rate : 1; b->cap = cap > 0 ? cap : 1;
    b->tokens = b->cap; b->last = 0;
}
/* Refill based on elapsed time and try to consume `cost`. now in seconds. */
int rl2_allow(RateBucket *b, long now, double cost) {
    if (!b) return 0;
    double elapsed = (double)(now - b->last);
    if (elapsed > 0) {
        b->tokens += elapsed * b->rate;
        if (b->tokens > b->cap) b->tokens = b->cap;
        b->last = now;
    }
    if (b->tokens >= cost) { b->tokens -= cost; return 1; }
    return 0;
}
