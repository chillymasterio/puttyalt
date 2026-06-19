/* puttyalt_easing.c - Easing interpolation functions (fixed inputs 0..1).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
double ea2_linear(double t) { return t; }
double ea2_in_quad(double t) { return t * t; }
double ea2_out_quad(double t) { return t * (2 - t); }
double ea2_in_out_quad(double t) { return t < 0.5 ? 2*t*t : -1 + (4 - 2*t)*t; }
double ea2_in_cubic(double t) { return t * t * t; }
double ea2_out_cubic(double t) { double f = t - 1; return f*f*f + 1; }
double ea2_lerp(double a, double b, double t) {
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return a + (b - a) * t;
}
