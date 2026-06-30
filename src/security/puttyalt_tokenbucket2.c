/* puttyalt_tokenbucket2.c - Token bucket for security throttling.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { double tokens; double capacity; double refill_rate; long last_refill; } TokenBucket2;
void tb4_init(TokenBucket2 *b, double capacity, double refill_rate) {
    if (!b) return;
    b->capacity = capacity > 0 ? capacity : 10;
    b->refill_rate = refill_rate > 0 ? refill_rate : 1;
    b->tokens = b->capacity; b->last_refill = 0;
}
int tb4_try_consume(TokenBucket2 *b, double n, long now) {
    if (!b) return 0;
    double elapsed = (double)(now - b->last_refill);
    if (elapsed > 0) {
        b->tokens += elapsed * b->refill_rate;
        if (b->tokens > b->capacity) b->tokens = b->capacity;
        b->last_refill = now;
    }
    if (b->tokens >= n) { b->tokens -= n; return 1; }
    return 0;
}
double tb4_available(const TokenBucket2 *b) { return b ? b->tokens : 0; }
