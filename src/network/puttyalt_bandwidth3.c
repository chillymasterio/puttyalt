/* puttyalt_bandwidth3.c - Rolling bandwidth estimate.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long bytes; long window_start; long window; long last_bps; } Bandwidth3;
void bw3_init(Bandwidth3 *b,long window,long now){ if(b){b->bytes=0;b->window_start=now;b->window=window>0?window:1;b->last_bps=0;} }
void bw3_add(Bandwidth3 *b,long bytes){ if(b)b->bytes+=bytes; }
long bw3_bps(Bandwidth3 *b,long now){ if(!b)return 0; if(now-b->window_start>=b->window){ b->last_bps=b->bytes/(now-b->window_start>0?now-b->window_start:1); b->bytes=0; b->window_start=now; } return b->last_bps; }
