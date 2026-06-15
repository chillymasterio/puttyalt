/* puttyalt_varint.c - LEB128 variable-length integer codec.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int vi_encode(uint64_t v, unsigned char *out, int outcap) {
    int o = 0;
    do {
        if (o >= outcap) return -1;
        unsigned char b = v & 0x7F; v >>= 7;
        if (v) b |= 0x80;
        out[o++] = b;
    } while (v);
    return o;
}
int vi_decode(const unsigned char *in, int n, uint64_t *out) {
    if (!in || !out) return -1;
    uint64_t v = 0; int shift = 0, i = 0;
    while (i < n) {
        unsigned char b = in[i++];
        v |= (uint64_t)(b & 0x7F) << shift;
        if (!(b & 0x80)) { *out = v; return i; }
        shift += 7;
        if (shift >= 64) return -1;
    }
    return -1;
}
