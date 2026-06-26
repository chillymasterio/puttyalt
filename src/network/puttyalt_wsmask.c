/* puttyalt_wsmask.c - WebSocket payload masking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
void wm4_apply(unsigned char *payload, int len, const unsigned char mask[4]) {
    if (!payload || !mask) return;
    for (int i = 0; i < len; i++) payload[i] ^= mask[i & 3];
}
/* Generate a deterministic mask from a seed (clients use random). */
void wm4_keygen(uint32_t seed, unsigned char mask[4]) {
    if (!mask) return;
    uint32_t x = seed * 2654435761u + 0x9e3779b9u;
    mask[0] = x & 0xFF; mask[1] = (x>>8)&0xFF; mask[2] = (x>>16)&0xFF; mask[3] = (x>>24)&0xFF;
}
