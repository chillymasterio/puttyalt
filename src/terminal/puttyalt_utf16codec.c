/* puttyalt_utf16codec.c - UTF-16 surrogate encode/decode.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int u16_encode(uint32_t cp, uint16_t *out) {
    if (cp < 0x10000) { out[0] = (uint16_t)cp; return 1; }
    if (cp <= 0x10FFFF) {
        cp -= 0x10000;
        out[0] = (uint16_t)(0xD800 | (cp >> 10));
        out[1] = (uint16_t)(0xDC00 | (cp & 0x3FF));
        return 2;
    }
    return -1;
}
int u16_decode(const uint16_t *in, int n, uint32_t *cp) {
    if (n < 1) return -1;
    uint16_t hi = in[0];
    if (hi >= 0xD800 && hi <= 0xDBFF) {
        if (n < 2) return -1;
        uint16_t lo = in[1];
        if (lo < 0xDC00 || lo > 0xDFFF) return -1;
        *cp = 0x10000 + (((uint32_t)(hi - 0xD800) << 10) | (lo - 0xDC00));
        return 2;
    }
    *cp = hi;
    return 1;
}
int u16_is_surrogate(uint16_t u) { return u >= 0xD800 && u <= 0xDFFF; }
