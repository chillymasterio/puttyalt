/* puttyalt_dnscache2.c - TTL DNS resolution cache.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define DNS_CAP 64
#define DNS_HOSTLEN 64
typedef struct { char host[DNS_HOSTLEN]; unsigned long ip; long expires; int used; } DnsEnt;
typedef struct { DnsEnt e[DNS_CAP]; } DnsCache2;
void dns2_init(DnsCache2 *c) { if (c) for (int i=0;i<DNS_CAP;i++) c->e[i].used=0; }
void dns2_put(DnsCache2 *c, const char *host, unsigned long ip, long now, long ttl) {
    if (!c || !host) return;
    int slot=-1; long oldest=0;
    for (int i=0;i<DNS_CAP;i++) {
        if (c->e[i].used && strcmp(c->e[i].host, host)==0) { slot=i; break; }
        if (!c->e[i].used) { if (slot<0) slot=i; }
        else if (c->e[i].expires < oldest || oldest==0) { if (slot<0||!c->e[slot].used) {} }
    }
    if (slot<0) slot=0;
    strncpy(c->e[slot].host, host, DNS_HOSTLEN-1); c->e[slot].host[DNS_HOSTLEN-1]=0;
    c->e[slot].ip=ip; c->e[slot].expires=now+ttl; c->e[slot].used=1;
}
int dns2_get(DnsCache2 *c, const char *host, long now, unsigned long *ip) {
    if (!c || !host) return -1;
    for (int i=0;i<DNS_CAP;i++)
        if (c->e[i].used && strcmp(c->e[i].host, host)==0) {
            if (now > c->e[i].expires) { c->e[i].used=0; return -1; }
            if (ip) *ip=c->e[i].ip;
            return 0;
        }
    return -1;
}
