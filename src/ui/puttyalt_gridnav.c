/* puttyalt_gridnav.c - Grid navigation with wrapping.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int rows, cols, row, col; int wrap; } GridNav;
void gnv_init(GridNav *g, int rows, int cols, int wrap){ if(g){g->rows=rows;g->cols=cols;g->row=0;g->col=0;g->wrap=wrap;} }
void gnv_move(GridNav *g, int dr, int dc){
    if(!g)return;
    g->row+=dr; g->col+=dc;
    if(g->wrap){ if(g->rows>0){g->row=(g->row%g->rows+g->rows)%g->rows;} if(g->cols>0){g->col=(g->col%g->cols+g->cols)%g->cols;} }
    else { if(g->row<0)g->row=0; if(g->row>=g->rows)g->row=g->rows-1; if(g->col<0)g->col=0; if(g->col>=g->cols)g->col=g->cols-1; }
}
int gnv_index(const GridNav *g){ return g?g->row*g->cols+g->col:-1; }
