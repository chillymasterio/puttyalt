/* puttyalt_keepalive5.c - Adaptive keepalive with failure tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long interval; long base; long max; int failures; } Keepalive5;
void ka5_init(Keepalive5 *k,long base,long max){ if(k){k->base=base>0?base:30;k->max=max>0?max:300;k->interval=k->base;k->failures=0;} }
void ka5_success(Keepalive5 *k){ if(k){k->failures=0;k->interval=k->base;} }
long ka5_failure(Keepalive5 *k){ if(!k)return 0; k->failures++; k->interval*=2; if(k->interval>k->max)k->interval=k->max; return k->interval; }
