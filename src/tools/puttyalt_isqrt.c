/* puttyalt_isqrt.c - Integer square root and power checks.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t iq_sqrt(uint64_t n) {
    if (n == 0) return 0;
    uint64_t x = n, y = (x + 1) / 2;
    while (y < x) { x = y; y = (x + n / x) / 2; }
    return (uint32_t)x;
}
int iq_is_square(uint64_t n) { uint32_t r = iq_sqrt(n); return (uint64_t)r * r == n; }
int iq_is_pow2(uint64_t n) { return n && !(n & (n - 1)); }
int iq_log2(uint64_t n) { int l = -1; while (n) { n >>= 1; l++; } return l; }
