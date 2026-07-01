/* puttyalt_lerpset.c - Interpolation helpers (linear/smooth/step).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
double lps_linear(double a, double b, double t) { return a + (b - a) * t; }
double lps_smoothstep(double t) { if (t<0)t=0; if(t>1)t=1; return t*t*(3-2*t); }
double lps_smootherstep(double t) { if (t<0)t=0; if(t>1)t=1; return t*t*t*(t*(t*6-15)+10); }
double lps_inverse(double a, double b, double v) { return (b==a)?0:(v-a)/(b-a); }
