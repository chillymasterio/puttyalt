/* puttyalt_hashcombine.c - Combine two 32-bit hashes with mix constants. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

uint32_t hashcombine_hash(const unsigned char *data, size_t len) {
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        h ^= data[i];
        h *= 16777619u;
    }
    return h;
}
uint32_t hashcombine_hash_str(const char *s) {
    return s ? hashcombine_hash((const unsigned char *)s, strlen(s)) : 0u;
}
int hashcombine_equal(const unsigned char *a, size_t alen,
              const unsigned char *b, size_t blen) {
    if (alen != blen) return 0;
    unsigned d = 0;
    for (size_t i = 0; i < alen; i++) d |= (unsigned)(a[i] ^ b[i]);
    return d == 0;
}
