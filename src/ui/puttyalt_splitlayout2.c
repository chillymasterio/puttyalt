/* puttyalt_splitlayout2.c - Two-pane split layout math.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int a_size; int b_size; } SplitResult;
SplitResult spl_compute(int total,int ratio_pct,int gap,int min_a,int min_b){
    SplitResult r; int avail=total-gap; r.a_size=avail*ratio_pct/100; r.b_size=avail-r.a_size;
    if(r.a_size<min_a){r.a_size=min_a;r.b_size=avail-min_a;}
    if(r.b_size<min_b){r.b_size=min_b;r.a_size=avail-min_b;}
    return r;
}
