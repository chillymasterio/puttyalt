/* puttyalt_scpprog.c - SCP transfer progress + ETA.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long total; long done; long start_time; } ScpProgress;
void scpp_init(ScpProgress *p,long total,long now){ if(p){p->total=total;p->done=0;p->start_time=now;} }
void scpp_update(ScpProgress *p,long done){ if(p)p->done=done; }
int scpp_percent(const ScpProgress *p){ return (p&&p->total>0)?(int)(p->done*100/p->total):0; }
long scpp_eta(const ScpProgress *p,long now){ if(!p||p->done==0)return -1; long el=now-p->start_time; if(el<=0)return -1; long rate=p->done/el; if(rate==0)return -1; return (p->total-p->done)/rate; }
