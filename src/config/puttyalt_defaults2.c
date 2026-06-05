/* puttyalt_defaults2.c - Layered config defaults (built-in < global < session). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DF_MAX 48
#define DF_KEY 40
#define DF_VAL 96
enum df_layer { DF_BUILTIN=0, DF_GLOBAL=1, DF_SESSION=2 };
typedef struct { char key[DF_KEY]; char val[DF_VAL]; int layer; } df_entry;
typedef struct { df_entry e[DF_MAX]; int n; } Defaults2;
void defaults2_init(Defaults2 *d) { if(d) memset(d,0,sizeof(*d)); }
int defaults2_set(Defaults2 *d, const char *key, const char *val, int layer) {
    if(!d||!key) return -1;
    for (int i=0;i<d->n;i++) if (strcmp(d->e[i].key,key)==0 && d->e[i].layer==layer) {
        snprintf(d->e[i].val,DF_VAL,"%s",val?val:""); return 0;
    }
    if (d->n>=DF_MAX) return -1;
    snprintf(d->e[d->n].key,DF_KEY,"%s",key); snprintf(d->e[d->n].val,DF_VAL,"%s",val?val:""); d->e[d->n].layer=layer; d->n++; return 0;
}
const char *defaults2_resolve(const Defaults2 *d, const char *key) {
    if(!d||!key) return 0; const char *best=0; int best_layer=-1;
    for (int i=0;i<d->n;i++) if (strcmp(d->e[i].key,key)==0 && d->e[i].layer>best_layer) { best_layer=d->e[i].layer; best=d->e[i].val; }
    return best;
}
int defaults2_resolve_int(const Defaults2 *d, const char *key, int fallback) {
    const char *v=defaults2_resolve(d,key); return v?atoi(v):fallback;
}
int defaults2_count(const Defaults2 *d) { return d?d->n:-1; }
