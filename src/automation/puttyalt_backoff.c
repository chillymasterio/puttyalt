/* puttyalt_backoff.c - Exponential backoff with jitter bounds.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long base, max; int attempt; } Backoff;
void bo_init(Backoff *b, long base, long max) {
    if (!b) return;
    b->base = base > 0 ? base : 1; b->max = max > 0 ? max : b->base; b->attempt = 0;
}
/* Returns delay for the current attempt then advances. Capped at max. */
long bo_next(Backoff *b) {
    if (!b) return 0;
    long d = b->base;
    for (int i = 0; i < b->attempt && d < b->max; i++) d <<= 1;
    if (d > b->max) d = b->max;
    b->attempt++;
    return d;
}
void bo_reset(Backoff *b) { if (b) b->attempt = 0; }
/* Deterministic jitter: returns d scaled by (50..100)% using a seed. */
long bo_jitter(long d, unsigned seed) {
    unsigned r = seed * 1103515245u + 12345u;
    int pct = 50 + (int)((r >> 16) % 51);
    return d * pct / 100;
}
