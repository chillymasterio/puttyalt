/* puttyalt_mergeconf.c - Merge config layers with overrides.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MC_CAP 64
#define MC_KLEN 48
#define MC_VLEN 96
typedef struct { char key[MC_KLEN]; char val[MC_VLEN]; int used; } McEnt;
typedef struct { McEnt e[MC_CAP]; } MergeConf;
void mc2_init(MergeConf *m) { if (m) for (int i=0;i<MC_CAP;i++) m->e[i].used=0; }
/* Set wins over existing (override semantics). */
int mc2_set(MergeConf *m, const char *k, const char *v) {
    if (!m || !k || !v) return -1;
    int slot = -1;
    for (int i=0;i<MC_CAP;i++) {
        if (m->e[i].used && strcmp(m->e[i].key,k)==0) { slot=i; break; }
        if (!m->e[i].used && slot<0) slot=i;
    }
    if (slot<0) return -1;
    strncpy(m->e[slot].key,k,MC_KLEN-1); m->e[slot].key[MC_KLEN-1]=0;
    strncpy(m->e[slot].val,v,MC_VLEN-1); m->e[slot].val[MC_VLEN-1]=0;
    m->e[slot].used=1;
    return 0;
}
/* Merge `over` on top of `base` into `base`. */
int mc2_merge(MergeConf *base, const MergeConf *over) {
    if (!base || !over) return -1;
    for (int i=0;i<MC_CAP;i++) if (over->e[i].used) mc2_set(base, over->e[i].key, over->e[i].val);
    return 0;
}
const char *mc2_get(const MergeConf *m, const char *k) {
    if (!m||!k) return 0;
    for (int i=0;i<MC_CAP;i++) if (m->e[i].used && strcmp(m->e[i].key,k)==0) return m->e[i].val;
    return 0;
}
