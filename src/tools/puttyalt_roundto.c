/* puttyalt_roundto.c - Round numbers to steps and significant digits.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <math.h>
double rnd_to_step(double v, double step) { if (step<=0) return v; return step * floor(v/step + 0.5); }
double rnd_to_places(double v, int places) {
    double m = 1; for (int i=0;i<places;i++) m*=10;
    return floor(v*m + 0.5) / m;
}
long rnd_nearest_pow2(long v) {
    if (v <= 0) return 1;
    long p = 1; while (p < v) p <<= 1;
    return (p - v < v - (p>>1)) ? p : (p>>1);
}
