/* puttyalt_weightedpick.c - Weighted random selection (deterministic).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int wpk_pick(const int *weights, int n, uint32_t *seed){
    if(!weights||n<=0||!seed)return -1;
    long total=0; for(int i=0;i<n;i++) total+=weights[i]>0?weights[i]:0;
    if(total==0)return 0;
    *seed=*seed*1103515245u+12345u;
    long r=(long)((*seed>>16)%total);
    for(int i=0;i<n;i++){ r-=weights[i]>0?weights[i]:0; if(r<0)return i; }
    return n-1;
}
int wpk_max(const int *weights, int n){ int best=0; for(int i=1;i<n;i++) if(weights[i]>weights[best])best=i; return best; }
