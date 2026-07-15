/* puttyalt_ratelimit3.c - Fixed-window rate limiter.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int count; long window_start; long window; int limit; } RateLimit3;
void rl7_init(RateLimit3 *r,int limit,long window){ if(r){r->count=0;r->window_start=0;r->window=window>0?window:60;r->limit=limit>0?limit:100;} }
int rl7_allow(RateLimit3 *r,long now){ if(!r)return 0; if(now-r->window_start>=r->window){r->window_start=now;r->count=0;} if(r->count>=r->limit)return 0; r->count++; return 1; }
int rl7_remaining(const RateLimit3 *r){ return r?r->limit-r->count:0; }
