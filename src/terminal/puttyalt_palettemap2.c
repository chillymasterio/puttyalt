/* puttyalt_palettemap2.c - Map indexed colors through a remap table.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define PM_SIZE 256
typedef struct { int map[PM_SIZE]; } PaletteMap;
void pmp_identity(PaletteMap *p){ if(!p)return; for(int i=0;i<PM_SIZE;i++)p->map[i]=i; }
void pmp_set(PaletteMap *p,int from,int to){ if(p&&from>=0&&from<PM_SIZE)p->map[from]=to; }
int pmp_apply(const PaletteMap *p,int color){ if(!p||color<0||color>=PM_SIZE)return color; return p->map[color]; }
