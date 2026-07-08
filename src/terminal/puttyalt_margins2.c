/* puttyalt_margins2.c - Horizontal/vertical margin regions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int top,bottom,left,right; int rows,cols; } Margins2;
void mgn_init(Margins2 *m,int rows,int cols){ if(!m)return; m->rows=rows;m->cols=cols;m->top=0;m->bottom=rows-1;m->left=0;m->right=cols-1; }
int mgn_set_vertical(Margins2 *m,int top,int bottom){ if(!m||top>=bottom||bottom>=m->rows)return -1; m->top=top;m->bottom=bottom;return 0; }
int mgn_inside(const Margins2 *m,int r,int c){ return m&&r>=m->top&&r<=m->bottom&&c>=m->left&&c<=m->right; }
