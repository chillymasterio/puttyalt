/* puttyalt_cidrlist.c - Match an IP against multiple CIDRs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <stdio.h>
#define CL_MAX 32
typedef struct { uint32_t net; int bits; } ClEntry;
typedef struct { ClEntry e[CL_MAX]; int n; } CidrList;
void clst_init(CidrList *l){ if(l)l->n=0; }
int clst_add(CidrList *l, const char *cidr){
    if(!l||l->n>=CL_MAX)return -1;
    unsigned a,b,c,d,m;
    if(sscanf(cidr,"%u.%u.%u.%u/%u",&a,&b,&c,&d,&m)!=5||m>32)return -1;
    l->e[l->n].net=(a<<24)|(b<<16)|(c<<8)|d; l->e[l->n].bits=m; l->n++;
    return 0;
}
int clst_contains(const CidrList *l, uint32_t ip){
    if(!l)return 0;
    for(int i=0;i<l->n;i++){ uint32_t mask=l->e[i].bits?~((1u<<(32-l->e[i].bits))-1):0; if((l->e[i].net&mask)==(ip&mask))return 1; }
    return 0;
}
