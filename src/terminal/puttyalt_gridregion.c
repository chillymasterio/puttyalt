/* puttyalt_gridregion.c - Rectangular cell-region operations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int r0,c0,r1,c1; } GridRegion;
int grg_valid(GridRegion g){ return g.r0<=g.r1 && g.c0<=g.c1; }
int grg_contains(GridRegion g, int r, int c){ return r>=g.r0&&r<=g.r1&&c>=g.c0&&c<=g.c1; }
int grg_cells(GridRegion g){ return grg_valid(g)?(g.r1-g.r0+1)*(g.c1-g.c0+1):0; }
GridRegion grg_clip(GridRegion g, int rows, int cols){
    if(g.r0<0)g.r0=0;
    if(g.c0<0)g.c0=0;
    if(g.r1>=rows)g.r1=rows-1;
    if(g.c1>=cols)g.c1=cols-1;
    return g;
}
