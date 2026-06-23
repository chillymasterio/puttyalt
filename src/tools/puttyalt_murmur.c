/* puttyalt_murmur.c - MurmurHash3 32-bit.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
static uint32_t mm4_rotl(uint32_t x, int8_t r) { return (x << r) | (x >> (32 - r)); }
uint32_t mm4_hash(const void *key, int len, uint32_t seed) {
    const uint8_t *data = key;
    const int nblocks = len / 4;
    uint32_t h = seed;
    const uint32_t c1 = 0xcc9e2d51, c2 = 0x1b873593;
    for (int i = 0; i < nblocks; i++) {
        uint32_t k;
        k = (uint32_t)data[i*4] | ((uint32_t)data[i*4+1] << 8) |
            ((uint32_t)data[i*4+2] << 16) | ((uint32_t)data[i*4+3] << 24);
        k *= c1; k = mm4_rotl(k, 15); k *= c2;
        h ^= k; h = mm4_rotl(h, 13); h = h * 5 + 0xe6546b64;
    }
    const uint8_t *tail = data + nblocks * 4;
    uint32_t k1 = 0;
    switch (len & 3) {
        case 3: k1 ^= (uint32_t)tail[2] << 16; /* fall through */
        case 2: k1 ^= (uint32_t)tail[1] << 8;  /* fall through */
        case 1: k1 ^= tail[0]; k1 *= c1; k1 = mm4_rotl(k1, 15); k1 *= c2; h ^= k1;
    }
    h ^= len;
    h ^= h >> 16; h *= 0x85ebca6b; h ^= h >> 13; h *= 0xc2b2ae35; h ^= h >> 16;
    return h;
}
