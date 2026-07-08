/* puttyalt_charset3.c - G0/G1 charset designation tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { char g0; char g1; int gl; } Charset3;
void cst_init(Charset3 *c){ if(c){c->g0='B';c->g1='B';c->gl=0;} }
void cst_designate(Charset3 *c,int slot,char set){ if(!c)return; if(slot==0)c->g0=set; else c->g1=set; }
void cst_shift(Charset3 *c,int gl){ if(c)c->gl=gl?1:0; }
char cst_active(const Charset3 *c){ if(!c)return 'B'; return c->gl?c->g1:c->g0; }
int cst_is_graphics(const Charset3 *c){ return cst_active(c)=='0'; }
