/* puttyalt_hostkey2.c - Track and compare known host keys.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define HK_MAX 32
typedef struct { char host[64]; char fp[80]; } HkEntry;
typedef struct { HkEntry e[HK_MAX]; int n; } HostKeys2;
void hk2_init(HostKeys2 *h){ if(h)h->n=0; }
int hk2_add(HostKeys2 *h, const char *host, const char *fp){
    if(!h||h->n>=HK_MAX)return -1;
    strncpy(h->e[h->n].host,host,63); h->e[h->n].host[63]=0;
    strncpy(h->e[h->n].fp,fp,79); h->e[h->n].fp[79]=0; h->n++;
    return 0;
}
/* Returns 1 match, 0 unknown, -1 MISMATCH (host known, fp differs). */
int hk2_verify(const HostKeys2 *h, const char *host, const char *fp){
    if(!h)return 0;
    for(int i=0;i<h->n;i++) if(!strcmp(h->e[i].host,host)) return strcmp(h->e[i].fp,fp)==0?1:-1;
    return 0;
}
