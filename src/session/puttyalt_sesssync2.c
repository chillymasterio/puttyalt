/* puttyalt_sesssync2.c - Session sync v2: CRDT-style last-write-wins register map. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SY_MAX 64
#define SY_KEY 48
#define SY_VAL 160
typedef struct { char key[SY_KEY]; char val[SY_VAL]; uint64_t timestamp; char origin[24]; } sy_reg;
typedef struct { sy_reg r[SY_MAX]; int n; char node_id[24]; } SessSync2;
void sesssync2_init(SessSync2 *s, const char *node_id) {
    if(!s) return; memset(s,0,sizeof(*s)); snprintf(s->node_id,24,"%s",node_id?node_id:"");
}
int sesssync2_set(SessSync2 *s, const char *key, const char *val, uint64_t ts) {
    if(!s||!key) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->r[i].key,key)==0) {
        if (ts>s->r[i].timestamp || (ts==s->r[i].timestamp && strcmp(s->node_id,s->r[i].origin)>0)) {
            snprintf(s->r[i].val,SY_VAL,"%s",val?val:""); s->r[i].timestamp=ts; snprintf(s->r[i].origin,24,"%s",s->node_id);
        }
        return 0;
    }
    if (s->n>=SY_MAX) return -1;
    sy_reg *r=&s->r[s->n++]; snprintf(r->key,SY_KEY,"%s",key); snprintf(r->val,SY_VAL,"%s",val?val:"");
    r->timestamp=ts; snprintf(r->origin,24,"%s",s->node_id); return 0;
}
int sesssync2_merge(SessSync2 *s, const char *key, const char *val, uint64_t ts, const char *origin) {
    if(!s||!key) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->r[i].key,key)==0) {
        if (ts>s->r[i].timestamp || (ts==s->r[i].timestamp && origin && strcmp(origin,s->r[i].origin)>0)) {
            snprintf(s->r[i].val,SY_VAL,"%s",val?val:""); s->r[i].timestamp=ts; snprintf(s->r[i].origin,24,"%s",origin?origin:"");
            return 1; /* changed */
        }
        return 0;
    }
    if (s->n>=SY_MAX) return -1;
    sy_reg *r=&s->r[s->n++]; snprintf(r->key,SY_KEY,"%s",key); snprintf(r->val,SY_VAL,"%s",val?val:"");
    r->timestamp=ts; snprintf(r->origin,24,"%s",origin?origin:""); return 1;
}
const char *sesssync2_get(const SessSync2 *s, const char *key) {
    if(!s||!key) return NULL;
    for (int i=0;i<s->n;i++) if (strcmp(s->r[i].key,key)==0) return s->r[i].val;
    return NULL;
}
int sesssync2_count(const SessSync2 *s) { return s?s->n:-1; }
