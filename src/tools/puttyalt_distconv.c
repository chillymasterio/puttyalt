/* puttyalt_distconv.c - Distance unit conversions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
double dst_km2mi(double km){ return km*0.621371; }
double dst_mi2km(double mi){ return mi*1.609344; }
double dst_m2ft(double m){ return m*3.28084; }
double dst_ft2m(double ft){ return ft/3.28084; }
