/* puttyalt_jitterbo.c - Exponential backoff with proportional jitter.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
typedef struct { long base; long cap; int attempt; } JitterBo;
void jbo_init(JitterBo *b, long base, long cap) { if (b){ b->base=base>0?base:1; b->cap=cap>0?cap:60; b->attempt=0; } }
long jbo_next(JitterBo *b, uint32_t *seed) {
    if (!b) return 0;
    long d = b->base;
    for (int i=0;i<b->attempt && d<b->cap;i++) d<<=1;
    if (d>b->cap) d=b->cap;
    b->attempt++;
    if (seed) { *seed = *seed*1103515245u+12345u; long j=(long)((*seed>>16)%(d/2+1)); return d - d/4 + j; }
    return d;
}
void jbo_reset(JitterBo *b) { if (b) b->attempt=0; }
