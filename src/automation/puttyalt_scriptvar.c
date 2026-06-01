/* puttyalt_scriptvar.c - Script variable scope stack for automation runner. */
#include <string.h>
#include <stdio.h>
#define SV_MAX 64
#define SV_NAME 40
#define SV_VAL 160
typedef struct { char name[SV_NAME]; char val[SV_VAL]; int scope; } sv_var;
typedef struct { sv_var v[SV_MAX]; int n; int scope; } ScriptVar;
void scriptvar_init(ScriptVar *s) { if(s) memset(s,0,sizeof(*s)); }
void scriptvar_push_scope(ScriptVar *s) { if(s) s->scope++; }
int scriptvar_pop_scope(ScriptVar *s) {
    if(!s||s->scope==0) return -1;
    int w=0; for (int i=0;i<s->n;i++) if (s->v[i].scope<s->scope) s->v[w++]=s->v[i];
    int removed=s->n-w; s->n=w; s->scope--; return removed;
}
int scriptvar_set(ScriptVar *s, const char *name, const char *val) {
    if(!s||!name) return -1;
    for (int i=s->n-1;i>=0;i--) if (strcmp(s->v[i].name,name)==0) { snprintf(s->v[i].val,SV_VAL,"%s",val?val:""); return 0; }
    if (s->n>=SV_MAX) return -1;
    sv_var *v=&s->v[s->n++]; snprintf(v->name,SV_NAME,"%s",name); snprintf(v->val,SV_VAL,"%s",val?val:""); v->scope=s->scope; return 0;
}
const char *scriptvar_get(const ScriptVar *s, const char *name) {
    if(!s||!name) return NULL;
    for (int i=s->n-1;i>=0;i--) if (strcmp(s->v[i].name,name)==0) return s->v[i].val;
    return NULL;
}
int scriptvar_count(const ScriptVar *s) { return s?s->n:-1; }
