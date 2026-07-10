/* puttyalt_pipeline3.c - Multi-stage pipeline with status.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define PL_STAGES 12
typedef struct { char name[24]; int status; } Pl3Stage;
typedef struct { Pl3Stage stages[PL_STAGES]; int n; int current; } Pipeline3;
void pl3_init(Pipeline3 *p){ if(p){p->n=0;p->current=0;} }
int pl3_add(Pipeline3 *p,const char *name){ if(!p||p->n>=PL_STAGES)return -1; strncpy(p->stages[p->n].name,name,23); p->stages[p->n].name[23]=0; p->stages[p->n].status=-1; return p->n++; }
int pl3_step(Pipeline3 *p,int code){ if(!p||p->current>=p->n)return -1; p->stages[p->current].status=code; if(code!=0)return -1; p->current++; return p->current<p->n?0:1; }
