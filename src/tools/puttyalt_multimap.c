/* puttyalt_multimap.c - Map keys to multiple integer values.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MM_KEYS 32
#define MM_VALS 8
typedef struct { char key[32]; int vals[MM_VALS]; int nvals; } MmEntry;
typedef struct { MmEntry e[MM_KEYS]; int n; } MultiMap;
void mmp_init(MultiMap *m){ if(m)m->n=0; }
int mmp_add(MultiMap *m, const char *key, int val){
    if(!m||!key)return -1;
    for(int i=0;i<m->n;i++) if(!strcmp(m->e[i].key,key)){ if(m->e[i].nvals<MM_VALS){m->e[i].vals[m->e[i].nvals++]=val;return 0;} return -1; }
    if(m->n>=MM_KEYS)return -1;
    strncpy(m->e[m->n].key,key,31); m->e[m->n].key[31]=0; m->e[m->n].vals[0]=val; m->e[m->n].nvals=1; m->n++;
    return 0;
}
int mmp_get(const MultiMap *m, const char *key, int *out, int cap){
    if(!m||!key)return 0;
    for(int i=0;i<m->n;i++) if(!strcmp(m->e[i].key,key)){ int c=m->e[i].nvals<cap?m->e[i].nvals:cap; for(int j=0;j<c;j++)out[j]=m->e[i].vals[j]; return c; }
    return 0;
}
