/* puttyalt_clamp2.c - Clamp and remap numeric ranges.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int cl2_clampi(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }
double cl2_clampf(double v, double lo, double hi) { return v < lo ? lo : (v > hi ? hi : v); }
/* Remap v from [in_lo,in_hi] to [out_lo,out_hi]. */
double cl2_remap(double v, double in_lo, double in_hi, double out_lo, double out_hi) {
    if (in_hi == in_lo) return out_lo;
    double t = (v - in_lo) / (in_hi - in_lo);
    return out_lo + t * (out_hi - out_lo);
}
int cl2_wrap(int v, int lo, int hi) {
    int range = hi - lo;
    if (range <= 0) return lo;
    int m = (v - lo) % range;
    if (m < 0) m += range;
    return lo + m;
}
