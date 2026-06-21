/* puttyalt_ratio2.c - Reduce and format fractions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
static long rat_gcd(long a, long b) { while (b) { long t = a % b; a = b; b = t; } return a < 0 ? -a : a; }
typedef struct { long num, den; } Ratio;
Ratio rat_reduce(long num, long den) {
    Ratio r = {num, den};
    if (den == 0) return r;
    long g = rat_gcd(num, den);
    if (g > 0) { r.num = num / g; r.den = den / g; }
    if (r.den < 0) { r.num = -r.num; r.den = -r.den; }
    return r;
}
int rat_format(Ratio r, char *out, int outlen) {
    if (r.den == 1) return snprintf(out, outlen, "%ld", r.num);
    return snprintf(out, outlen, "%ld/%ld", r.num, r.den);
}
int rat_percent(Ratio r) { return r.den ? (int)(r.num * 100 / r.den) : 0; }
