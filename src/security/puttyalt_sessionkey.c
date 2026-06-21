/* puttyalt_sessionkey.c - Derive and rotate session keys.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
typedef struct { uint64_t key; int generation; uint64_t seed; } SessionKey;
static uint64_t sk6_mix(uint64_t x) {
    x ^= x >> 30; x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27; x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}
void sk6_init(SessionKey *k, uint64_t seed) {
    if (!k) return;
    k->seed = seed; k->generation = 0; k->key = sk6_mix(seed);
}
/* Rotate to a new key derived from the previous one. */
uint64_t sk6_rotate(SessionKey *k) {
    if (!k) return 0;
    k->generation++;
    k->key = sk6_mix(k->key ^ ((uint64_t)k->generation * 0x9e3779b97f4a7c15ULL));
    return k->key;
}
int sk6_generation(const SessionKey *k) { return k ? k->generation : -1; }
