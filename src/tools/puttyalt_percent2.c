/* puttyalt_percent2.c - Percentage and ratio calculations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
double pct_of(double part,double whole){ return whole!=0?part/whole*100.0:0; }
double pct_change(double from,double to){ return from!=0?(to-from)/from*100.0:0; }
double pct_apply(double value,double percent){ return value*percent/100.0; }
int pct_bar_fill(int percent,int width){ if(percent<0)percent=0; if(percent>100)percent=100; return percent*width/100; }
