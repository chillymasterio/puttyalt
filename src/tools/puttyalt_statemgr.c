/* puttyalt_statemgr.c - Application state manager with persistence flags. */
#include <string.h>
#include <stdio.h>
#define SM_MAX 48
#define SM_KEY 40
#define SM_VAL 96
typedef struct { char key[SM_KEY]; char value[SM_VAL]; int persistent; int dirty; } sm_state;
typedef struct { sm_state states[SM_MAX]; int n; } StateMgr;
void statemgr_init(StateMgr *s) { if(s) memset(s,0,sizeof(*s)); }
int statemgr_set(StateMgr *s, const char *key, const char *value, int persistent) {
    if(!s||!key) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->states[i].key,key)==0) {
        if (strcmp(s->states[i].value,value?value:"")!=0) { snprintf(s->states[i].value,SM_VAL,"%s",value?value:""); s->states[i].dirty=1; }
        return 0;
    }
    if (s->n>=SM_MAX) return -1;
    sm_state *st=&s->states[s->n++]; snprintf(st->key,SM_KEY,"%s",key); snprintf(st->value,SM_VAL,"%s",value?value:"");
    st->persistent=persistent?1:0; st->dirty=1; return 0;
}
const char *statemgr_get(const StateMgr *s, const char *key) {
    if(!s||!key) return 0;
    for (int i=0;i<s->n;i++) if (strcmp(s->states[i].key,key)==0) return s->states[i].value;
    return 0;
}
int statemgr_export_persistent(const StateMgr *s, char *out, int outlen) {
    if(!s||!out) return -1;
    int pos=0;
    for (int i=0;i<s->n && pos<outlen;i++) if (s->states[i].persistent)
        pos+=snprintf(out+pos,outlen-pos,"%s=%s\n",s->states[i].key,s->states[i].value);
    return pos;
}
int statemgr_dirty_count(const StateMgr *s) {
    if(!s) return -1;
    int n=0; for(int i=0;i<s->n;i++) if(s->states[i].dirty)n++; return n;
}
