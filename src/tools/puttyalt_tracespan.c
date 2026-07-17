/* puttyalt_tracespan.c - Trace span timing and nesting.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define TS_DEPTH 16
typedef struct { long starts[TS_DEPTH]; int depth; long total; } TraceSpan;
void trs_init(TraceSpan *t){ if(t){t->depth=0;t->total=0;} }
void trs_enter(TraceSpan *t,long now){ if(t&&t->depth<TS_DEPTH)t->starts[t->depth++]=now; }
long trs_exit(TraceSpan *t,long now){ if(!t||t->depth==0)return -1; long dur=now-t->starts[--t->depth]; if(t->depth==0)t->total+=dur; return dur; }
