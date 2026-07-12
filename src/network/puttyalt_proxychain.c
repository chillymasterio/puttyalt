/* puttyalt_proxychain.c - Ordered proxy hop chain.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define PC_MAX 8
typedef struct { char host[64]; int port; } PcHop;
typedef struct { PcHop hops[PC_MAX]; int n; } ProxyChain;
void pcc_init(ProxyChain *c){ if(c)c->n=0; }
int pcc_add(ProxyChain *c,const char *host,int port){ if(!c||c->n>=PC_MAX)return -1; strncpy(c->hops[c->n].host,host,63); c->hops[c->n].host[63]=0; c->hops[c->n].port=port; return c->n++; }
int pcc_count(const ProxyChain *c){ return c?c->n:0; }
