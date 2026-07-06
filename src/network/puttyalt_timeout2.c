/* puttyalt_timeout2.c - Track per-operation timeouts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long start; long timeout; int fired; } Timeout2;
void tmo_start(Timeout2 *t,long timeout,long now){ if(t){t->start=now;t->timeout=timeout;t->fired=0;} }
int tmo_expired(Timeout2 *t,long now){ if(!t||t->fired)return 0; if(now-t->start>=t->timeout){t->fired=1;return 1;} return 0; }
long tmo_remaining(const Timeout2 *t,long now){ if(!t)return 0; long r=t->timeout-(now-t->start); return r>0?r:0; }
