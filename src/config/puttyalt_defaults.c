/* puttyalt_defaults.c - Apply default values to missing keys.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define DF_CAP 48
#define DF_KLEN 40
#define DF_VLEN 80
typedef struct { char key[DF_KLEN]; char val[DF_VLEN]; int set; } DfEnt;
typedef struct { DfEnt e[DF_CAP]; int n; } DefaultSet;
void df2_init(DefaultSet *d) { if (d) d->n = 0; }
int df2_default(DefaultSet *d, const char *k, const char *v) {
    if (!d || d->n >= DF_CAP) return -1;
    strncpy(d->e[d->n].key, k, DF_KLEN-1); d->e[d->n].key[DF_KLEN-1]=0;
    strncpy(d->e[d->n].val, v, DF_VLEN-1); d->e[d->n].val[DF_VLEN-1]=0;
    d->e[d->n].set = 0;
    d->n++;
    return 0;
}
int df2_override(DefaultSet *d, const char *k, const char *v) {
    if (!d) return -1;
    for (int i = 0; i < d->n; i++) if (strcmp(d->e[i].key, k) == 0) {
        strncpy(d->e[i].val, v, DF_VLEN-1); d->e[i].val[DF_VLEN-1]=0; d->e[i].set = 1; return 0;
    }
    return -1;
}
const char *df2_get(const DefaultSet *d, const char *k, int *was_default) {
    if (!d) return 0;
    for (int i = 0; i < d->n; i++) if (strcmp(d->e[i].key, k) == 0) {
        if (was_default) *was_default = !d->e[i].set;
        return d->e[i].val;
    }
    return 0;
}
