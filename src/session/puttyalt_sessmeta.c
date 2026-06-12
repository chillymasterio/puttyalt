/* puttyalt_sessmeta.c - Session metadata store (custom key/value per session). */
#include <string.h>
#include <stdio.h>
#define SM_MAX 64
#define SM_KEY 32
#define SM_VAL 96
typedef struct { int session_id; char key[SM_KEY]; char value[SM_VAL]; } sm_meta;
typedef struct { sm_meta m[SM_MAX]; int n; } SessMeta;
void sessmeta_init(SessMeta *s) { if(s) memset(s,0,sizeof(*s)); }
int sessmeta_set(SessMeta *s, int session_id, const char *key, const char *value) {
    if(!s||!key) return -1;
    for (int i=0;i<s->n;i++) if (s->m[i].session_id==session_id && strcmp(s->m[i].key,key)==0) {
        snprintf(s->m[i].value,SM_VAL,"%s",value?value:""); return 0;
    }
    if (s->n>=SM_MAX) return -1;
    sm_meta *m=&s->m[s->n++]; m->session_id=session_id; snprintf(m->key,SM_KEY,"%s",key); snprintf(m->value,SM_VAL,"%s",value?value:"");
    return 0;
}
const char *sessmeta_get(const SessMeta *s, int session_id, const char *key) {
    if(!s||!key) return 0;
    for (int i=0;i<s->n;i++) if (s->m[i].session_id==session_id && strcmp(s->m[i].key,key)==0) return s->m[i].value;
    return 0;
}
int sessmeta_keys(const SessMeta *s, int session_id, int *out_idx, int cap) {
    if(!s) return -1;
    int n=0;
    for (int i=0;i<s->n && n<cap;i++) if (s->m[i].session_id==session_id) out_idx[n++]=i;
    return n;
}
int sessmeta_count(const SessMeta *s) { return s?s->n:-1; }
