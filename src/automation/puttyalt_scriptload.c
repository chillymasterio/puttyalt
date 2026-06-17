/* puttyalt_scriptload.c - Script loader with dependency resolution. */
#include <string.h>
#include <stdio.h>
#define SL_MAX 32
#define SL_NAME 48
typedef struct { char name[SL_NAME]; int dep_mask; int loaded; int load_order; } sl_script;
typedef struct { sl_script scripts[SL_MAX]; int n; int order_counter; } ScriptLoad;
void scriptload_init(ScriptLoad *s) { if(s) memset(s,0,sizeof(*s)); }
int scriptload_register(ScriptLoad *s, const char *name, int dep_mask) {
    if(!s||s->n>=SL_MAX||!name) return -1;
    sl_script *sc=&s->scripts[s->n]; snprintf(sc->name,SL_NAME,"%s",name); sc->dep_mask=dep_mask; sc->load_order=-1;
    return s->n++;
}
static int sl_loaded_mask(const ScriptLoad *s) {
    int m=0; for(int i=0;i<s->n;i++) if(s->scripts[i].loaded) m|=(1<<i); return m;
}
int scriptload_next(ScriptLoad *s) {
    if(!s) return -1;
    int loaded=sl_loaded_mask(s);
    for (int i=0;i<s->n;i++) if (!s->scripts[i].loaded && (s->scripts[i].dep_mask&loaded)==s->scripts[i].dep_mask) {
        s->scripts[i].loaded=1; s->scripts[i].load_order=s->order_counter++; return i;
    }
    return -1; /* none ready (or cyclic deps) */
}
int scriptload_all_loaded(const ScriptLoad *s) {
    if(!s) return 0;
    for(int i=0;i<s->n;i++) if(!s->scripts[i].loaded) return 0;
    return 1;
}
int scriptload_has_cycle(ScriptLoad *s) {
    if(!s) return 0;
    int loaded=sl_loaded_mask(s);
    for (int i=0;i<s->n;i++) if (!s->scripts[i].loaded && (s->scripts[i].dep_mask&loaded)==s->scripts[i].dep_mask) return 0;
    return scriptload_all_loaded(s)?0:1;
}
int scriptload_count(const ScriptLoad *s) { return s?s->n:-1; }
