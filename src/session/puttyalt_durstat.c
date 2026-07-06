/* puttyalt_durstat.c - Aggregate session durations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long total; long count; long longest; long shortest; int started; } DurStat;
void dst_init(DurStat *d){ if(d){d->total=0;d->count=0;d->longest=0;d->shortest=0;d->started=0;} }
void dst_add(DurStat *d,long dur){ if(!d)return; d->total+=dur; d->count++; if(!d->started){d->longest=d->shortest=dur;d->started=1;} else {if(dur>d->longest)d->longest=dur; if(dur<d->shortest)d->shortest=dur;} }
long dst_average(const DurStat *d){ return (d&&d->count)?d->total/d->count:0; }
