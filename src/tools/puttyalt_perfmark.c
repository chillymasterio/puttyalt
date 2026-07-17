/* puttyalt_perfmark.c - Named performance marks and measures.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define PF_MAX 16
typedef struct { char name[24]; long time; } PfMark;
typedef struct { PfMark marks[PF_MAX]; int n; } PerfMark;
void pfm_init(PerfMark *p){ if(p)p->n=0; }
void pfm_mark(PerfMark *p,const char *name,long now){ if(!p||p->n>=PF_MAX)return; strncpy(p->marks[p->n].name,name,23); p->marks[p->n].name[23]=0; p->marks[p->n].time=now; p->n++; }
long pfm_measure(const PerfMark *p,const char *from,const char *to){ if(!p)return -1; long a=-1,b=-1; for(int i=0;i<p->n;i++){ if(!strcmp(p->marks[i].name,from))a=p->marks[i].time; if(!strcmp(p->marks[i].name,to))b=p->marks[i].time; } return (a>=0&&b>=0)?b-a:-1; }
