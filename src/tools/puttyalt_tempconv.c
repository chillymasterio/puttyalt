/* puttyalt_tempconv.c - Temperature unit conversions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
double tmp_c2f(double c){ return c*9.0/5.0+32.0; }
double tmp_f2c(double f){ return (f-32.0)*5.0/9.0; }
double tmp_c2k(double c){ return c+273.15; }
double tmp_k2c(double k){ return k-273.15; }
