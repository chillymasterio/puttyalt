/* puttyalt_metriccnt.c - Named metric counters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MC_MAX 24
typedef struct { char name[24]; long value; } McCounter;
typedef struct { McCounter c[MC_MAX]; int n; } MetricCnt;
void mtc_init(MetricCnt *m){ if(m)m->n=0; }
void mtc_incr(MetricCnt *m,const char *name,long by){ if(!m)return; for(int i=0;i<m->n;i++)if(!strcmp(m->c[i].name,name)){m->c[i].value+=by;return;} if(m->n<MC_MAX){strncpy(m->c[m->n].name,name,23);m->c[m->n].name[23]=0;m->c[m->n].value=by;m->n++;} }
long mtc_get(const MetricCnt *m,const char *name){ if(!m)return 0; for(int i=0;i<m->n;i++)if(!strcmp(m->c[i].name,name))return m->c[i].value; return 0; }
