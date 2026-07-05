/* puttyalt_ipallow.c - IP allow/deny list evaluation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#define IA_MAX 32
typedef struct { uint32_t net; int bits; int allow; } IaRule;
typedef struct { IaRule r[IA_MAX]; int n; int default_allow; } IpAllow;
void ial_init(IpAllow *a, int default_allow){ if(a){a->n=0;a->default_allow=default_allow;} }
int ial_add(IpAllow *a, uint32_t net, int bits, int allow){ if(!a||a->n>=IA_MAX)return -1; a->r[a->n].net=net; a->r[a->n].bits=bits; a->r[a->n].allow=allow; a->n++; return 0; }
/* First matching rule wins; else default. */
int ial_check(const IpAllow *a, uint32_t ip){
    if(!a)return 0;
    for(int i=0;i<a->n;i++){ uint32_t mask=a->r[i].bits?~((1u<<(32-a->r[i].bits))-1):0; if((a->r[i].net&mask)==(ip&mask))return a->r[i].allow; }
    return a->default_allow;
}
