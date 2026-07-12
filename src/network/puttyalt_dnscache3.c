/* puttyalt_dnscache3.c - DNS cache with negative caching.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdint.h>
#define DC_MAX 32
typedef struct { char host[64]; uint32_t ip; long expires; int negative; int used; } Dc3Entry;
typedef struct { Dc3Entry e[DC_MAX]; } DnsCache3;
void dc3_init(DnsCache3 *c){ if(c)for(int i=0;i<DC_MAX;i++)c->e[i].used=0; }
void dc3_put(DnsCache3 *c,const char *host,uint32_t ip,int negative,long now,long ttl){ if(!c)return; for(int i=0;i<DC_MAX;i++)if(!c->e[i].used||!strcmp(c->e[i].host,host)){ strncpy(c->e[i].host,host,63);c->e[i].host[63]=0;c->e[i].ip=ip;c->e[i].negative=negative;c->e[i].expires=now+ttl;c->e[i].used=1;return;} }
int dc3_get(DnsCache3 *c,const char *host,long now,uint32_t *ip){ if(!c)return -1; for(int i=0;i<DC_MAX;i++)if(c->e[i].used&&!strcmp(c->e[i].host,host)){ if(now>c->e[i].expires){c->e[i].used=0;return -1;} if(c->e[i].negative)return -2; if(ip)*ip=c->e[i].ip; return 0; } return -1; }
