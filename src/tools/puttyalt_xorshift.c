/* puttyalt_xorshift.c - Xorshift pseudo-random generator.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
typedef struct { uint64_t state; } Xorshift;
void xs2_seed(Xorshift *r, uint64_t seed) { if (r) r->state = seed ? seed : 0x9e3779b97f4a7c15ULL; }
uint64_t xs2_next(Xorshift *r) {
    if (!r) return 0;
    uint64_t x = r->state;
    x ^= x << 13; x ^= x >> 7; x ^= x << 17;
    r->state = x;
    return x;
}
/* Bounded random in [0, bound). */
uint32_t xs2_range(Xorshift *r, uint32_t bound) {
    if (!r || bound == 0) return 0;
    return (uint32_t)(xs2_next(r) % bound);
}
double xs2_double(Xorshift *r) {
    return (double)(xs2_next(r) >> 11) / (double)(1ULL << 53);
}
