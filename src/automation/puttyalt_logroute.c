/* puttyalt_logroute.c - Route log entries to handlers by level.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define LGR_MAX 8
typedef struct { int min_level; int handler_id; } LrRoute;
typedef struct { LrRoute routes[LGR_MAX]; int n; } LogRoute;
void lgt_init(LogRoute *l){ if(l)l->n=0; }
int lgt_add(LogRoute *l,int min_level,int handler){ if(!l||l->n>=LGR_MAX)return -1; l->routes[l->n].min_level=min_level; l->routes[l->n].handler_id=handler; return l->n++; }
int lgt_route(const LogRoute *l,int level,int *out,int cap){ if(!l)return 0; int c=0; for(int i=0;i<l->n&&c<cap;i++)if(level>=l->routes[i].min_level)out[c++]=l->routes[i].handler_id; return c; }
