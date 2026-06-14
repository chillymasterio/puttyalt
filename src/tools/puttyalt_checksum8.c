/* puttyalt_checksum8.c - XOR / additive / Fletcher-16 checksums.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
unsigned char cs8_xor(const unsigned char *d, int n) {
    unsigned char c = 0;
    for (int i = 0; i < n; i++) c ^= d[i];
    return c;
}
unsigned char cs8_add(const unsigned char *d, int n) {
    unsigned int s = 0;
    for (int i = 0; i < n; i++) s += d[i];
    return (unsigned char)(s & 0xFF);
}
uint16_t cs8_fletcher16(const unsigned char *d, int n) {
    uint16_t a = 0, b = 0;
    for (int i = 0; i < n; i++) { a = (uint16_t)((a + d[i]) % 255); b = (uint16_t)((b + a) % 255); }
    return (uint16_t)((b << 8) | a);
}
