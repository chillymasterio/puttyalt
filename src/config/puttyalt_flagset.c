/* puttyalt_flagset.c - Named boolean flag set with bitmask.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define FS_MAX 32
typedef struct { char names[FS_MAX][24]; int n; unsigned long bits; } FlagSet;
void fst_init(FlagSet *f){ if(f){f->n=0;f->bits=0;} }
int fst_define(FlagSet *f,const char *name){ if(!f||f->n>=FS_MAX)return -1; strncpy(f->names[f->n],name,23); f->names[f->n][23]=0; return f->n++; }
static int fst_find(const FlagSet *f,const char *name){ for(int i=0;i<f->n;i++)if(!strcmp(f->names[i],name))return i; return -1; }
void fst_set(FlagSet *f,const char *name,int on){ int i=fst_find(f,name); if(i<0)return; if(on)f->bits|=(1UL<<i); else f->bits&=~(1UL<<i); }
int fst_get(const FlagSet *f,const char *name){ int i=fst_find(f,name); return i>=0&&(f->bits&(1UL<<i)); }
