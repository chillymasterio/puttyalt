/* puttyalt_consistenthash.c - Consistent hashing ring.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#define CH_MAX 64
typedef struct { uint32_t hash; int node; } ChPoint;
typedef struct { ChPoint points[CH_MAX]; int n; } ConsistentHash;
void chr_init(ConsistentHash *c){ if(c)c->n=0; }
static uint32_t chr_h(uint32_t x){ x^=x>>16; x*=0x85ebca6b; x^=x>>13; return x; }
int chr_add(ConsistentHash *c,int node,int vnodes){
    if(!c)return -1;
    for(int v=0;v<vnodes&&c->n<CH_MAX;v++){ c->points[c->n].hash=chr_h(node*1000+v); c->points[c->n].node=node; c->n++; }
    for(int i=0;i<c->n;i++)
        for(int j=i+1;j<c->n;j++)
            if(c->points[j].hash<c->points[i].hash){ChPoint t=c->points[i];c->points[i]=c->points[j];c->points[j]=t;}
    return 0;
}
int chr_lookup(const ConsistentHash *c,uint32_t key){ if(!c||c->n==0)return -1; for(int i=0;i<c->n;i++)if(c->points[i].hash>=key)return c->points[i].node; return c->points[0].node; }
