/* puttyalt_depresolve.c - Resolve a linear dependency order.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define DR_MAX 32
typedef struct { int deps[DR_MAX]; int ndeps; int resolved; } DrNode;
typedef struct { DrNode nodes[DR_MAX]; int n; } DepResolve;
void drs_init(DepResolve *d, int n){ if(d){d->n=n>DR_MAX?DR_MAX:n; for(int i=0;i<d->n;i++){d->nodes[i].ndeps=0;d->nodes[i].resolved=0;}} }
int drs_add(DepResolve *d, int node, int dep){ if(!d||node<0||node>=d->n||d->nodes[node].ndeps>=DR_MAX)return -1; d->nodes[node].deps[d->nodes[node].ndeps++]=dep; return 0; }
/* Produce resolution order into out[]; returns count, -1 on cycle. */
int drs_order(DepResolve *d, int *out, int cap){
    if(!d)return -1;
    int count=0, changed=1, passes=0;
    while(changed && passes++ <= d->n){
        changed=0;
        for(int i=0;i<d->n;i++) if(!d->nodes[i].resolved){
            int ready=1; for(int j=0;j<d->nodes[i].ndeps;j++) if(!d->nodes[d->nodes[i].deps[j]].resolved){ready=0;break;}
            if(ready){ d->nodes[i].resolved=1; if(count<cap)out[count++]=i; changed=1; }
        }
    }
    return count==d->n ? count : -1;
}
