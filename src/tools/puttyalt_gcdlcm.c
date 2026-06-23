/* puttyalt_gcdlcm.c - Greatest common divisor and LCM.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint64_t gl2_gcd(uint64_t a, uint64_t b) { while (b) { uint64_t t = a % b; a = b; b = t; } return a; }
uint64_t gl2_lcm(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) return 0;
    return a / gl2_gcd(a, b) * b;
}
/* Extended GCD: returns g, sets x,y so ax+by=g. */
long gl2_egcd(long a, long b, long *x, long *y) {
    if (b == 0) { if (x) *x = 1; if (y) *y = 0; return a; }
    long x1, y1;
    long g = gl2_egcd(b, a % b, &x1, &y1);
    if (x) *x = y1;
    if (y) *y = x1 - (a / b) * y1;
    return g;
}
