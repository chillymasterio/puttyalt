/* puttyalt_interval2.c - Fire actions on fixed intervals.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long interval; long next_fire; } Interval2;
void ivl_init(Interval2 *i,long interval,long now){ if(i){i->interval=interval>0?interval:1;i->next_fire=now+i->interval;} }
int ivl_due(Interval2 *i,long now){ if(!i)return 0; if(now>=i->next_fire){ while(i->next_fire<=now)i->next_fire+=i->interval; return 1; } return 0; }
long ivl_remaining(const Interval2 *i,long now){ return i?i->next_fire-now:0; }
