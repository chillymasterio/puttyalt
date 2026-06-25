/* puttyalt_bloomfilter.c - Bloom filter membership test.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdint.h>
#define BLOOM_BITS 1024
typedef struct { unsigned char bits[BLOOM_BITS/8]; } BloomFilter;
void bf3_init(BloomFilter *b) { if (b) memset(b->bits, 0, sizeof b->bits); }
static uint32_t bf3_h1(const char *s) { uint32_t h=2166136261u; for(;*s;s++){h^=(unsigned char)*s;h*=16777619u;} return h; }
static uint32_t bf3_h2(const char *s) { uint32_t h=5381; for(;*s;s++) h=((h<<5)+h)+(unsigned char)*s; return h; }
void bf3_add(BloomFilter *b, const char *key) {
    if (!b || !key) return;
    uint32_t a = bf3_h1(key), c = bf3_h2(key);
    for (int i = 0; i < 3; i++) {
        uint32_t idx = (a + i * c) % BLOOM_BITS;
        b->bits[idx/8] |= (unsigned char)(1 << (idx % 8));
    }
}
/* Returns 0 if definitely absent, 1 if possibly present. */
int bf3_maybe(const BloomFilter *b, const char *key) {
    if (!b || !key) return 0;
    uint32_t a = bf3_h1(key), c = bf3_h2(key);
    for (int i = 0; i < 3; i++) {
        uint32_t idx = (a + i * c) % BLOOM_BITS;
        if (!(b->bits[idx/8] & (1 << (idx % 8)))) return 0;
    }
    return 1;
}
