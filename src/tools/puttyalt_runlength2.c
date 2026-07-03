/* puttyalt_runlength2.c - Run-length aggregate over a stream.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int last; int run; int distinct; long total; int started; } RunLength2;
void rl6_init(RunLength2 *r){ if(r){r->run=0;r->distinct=0;r->total=0;r->started=0;} }
/* feed a value; returns run length that just ENDED, or 0 if continuing. */
int rl6_feed(RunLength2 *r, int value){
    if(!r)return 0;
    r->total++;
    if(!r->started){ r->started=1; r->last=value; r->run=1; r->distinct=1; return 0; }
    if(value==r->last){ r->run++; return 0; }
    int ended=r->run;
    r->last=value; r->run=1; r->distinct++;
    return ended;
}
