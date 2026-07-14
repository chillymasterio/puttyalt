/* puttyalt_prefixsum.c - Prefix sum array and range queries.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define PS_MAX 256
typedef struct { long sums[PS_MAX]; int n; } PrefixSum;
void psm_build(PrefixSum *p,const int *a,int n){ if(!p)return; p->n=n>PS_MAX?PS_MAX:n; p->sums[0]=0; for(int i=0;i<p->n;i++)p->sums[i+1]=p->sums[i]+a[i]; }
long psm_range(const PrefixSum *p,int lo,int hi){ if(!p||lo<0||hi>p->n||lo>hi)return 0; return p->sums[hi]-p->sums[lo]; }
