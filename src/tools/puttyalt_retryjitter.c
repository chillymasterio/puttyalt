/* puttyalt_retryjitter.c - Compute retry delays with decorrelated jitter.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Decorrelated jitter: next = min(cap, rand(base, prev*3)). Deterministic via seed. */
long rj2_next(long base, long cap, long prev, uint32_t *seed) {
    if (!seed) return base;
    *seed = *seed * 1103515245u + 12345u;
    long upper = prev * 3;
    if (upper < base) upper = base;
    long range = upper - base + 1;
    long delay = base + (long)((*seed >> 8) % (range > 0 ? range : 1));
    return delay > cap ? cap : delay;
}
long rj2_full_jitter(long base, long cap, int attempt, uint32_t *seed) {
    long exp = base;
    for (int i = 0; i < attempt && exp < cap; i++) exp <<= 1;
    if (exp > cap) exp = cap;
    if (!seed) return exp;
    *seed = *seed * 1103515245u + 12345u;
    return (long)((*seed >> 8) % (exp + 1));
}
