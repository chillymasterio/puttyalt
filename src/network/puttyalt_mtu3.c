/* puttyalt_mtu3.c - Path MTU discovery estimation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int lo; int hi; int current; int found; } Mtu3;
void mt3_init(Mtu3 *m){ if(m){m->lo=576;m->hi=9000;m->current=1500;m->found=0;} }
void mt3_ok(Mtu3 *m){ if(!m)return; m->lo=m->current; m->current=(m->lo+m->hi)/2; if(m->hi-m->lo<=8){m->found=1;m->current=m->lo;} }
void mt3_toobig(Mtu3 *m){ if(!m)return; m->hi=m->current-1; m->current=(m->lo+m->hi)/2; if(m->hi-m->lo<=8){m->found=1;m->current=m->lo;} }
