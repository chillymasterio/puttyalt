/* puttyalt_cellbuf.c - Terminal cell buffer with attributes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdint.h>
#define CB_CELLS 4096
typedef struct { uint32_t ch; unsigned short attr; } CbCell;
typedef struct { CbCell cells[CB_CELLS]; int cols; int rows; } CellBuf;
void cbf_init(CellBuf *b,int cols,int rows){ if(!b)return; b->cols=cols; b->rows=rows; for(int i=0;i<CB_CELLS;i++){b->cells[i].ch=' ';b->cells[i].attr=0;} }
int cbf_set(CellBuf *b,int r,int c,unsigned int ch,unsigned short attr){ if(!b||r<0||c<0||r>=b->rows||c>=b->cols)return -1; int idx=r*b->cols+c; if(idx>=CB_CELLS)return -1; b->cells[idx].ch=ch; b->cells[idx].attr=attr; return 0; }
unsigned int cbf_get(const CellBuf *b,int r,int c){ if(!b||r<0||c<0||r>=b->rows||c>=b->cols)return ' '; int idx=r*b->cols+c; return idx<CB_CELLS?b->cells[idx].ch:' '; }
