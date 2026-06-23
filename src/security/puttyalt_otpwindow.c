/* puttyalt_otpwindow.c - HOTP counter window verification.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Check a HOTP counter within a look-ahead window. Returns matched counter or -1. */
int64_t ow2_verify(uint64_t expected, uint64_t candidate, int lookahead) {
    if (lookahead < 0) lookahead = 0;
    for (int i = 0; i <= lookahead; i++)
        if (candidate == expected + i) return (int64_t)(expected + i);
    return -1;
}
/* Truncate an HMAC value to N digits (dynamic truncation). */
uint32_t ow2_truncate(const uint8_t *hmac, int len, int digits) {
    if (!hmac || len < 20) return 0;
    int offset = hmac[len - 1] & 0x0f;
    uint32_t bin = ((hmac[offset] & 0x7f) << 24) | (hmac[offset+1] << 16) |
                   (hmac[offset+2] << 8) | hmac[offset+3];
    uint32_t mod = 1;
    for (int i = 0; i < digits; i++) mod *= 10;
    return bin % mod;
}
