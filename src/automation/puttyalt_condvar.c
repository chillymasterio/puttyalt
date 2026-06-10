/* puttyalt_condvar.c - Conditional variable expansion for automation. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define CV_MAX 32
#define CV_NAME 32
#define CV_VAL 96
typedef struct { char name[CV_NAME]; char value[CV_VAL]; } cv_var;
typedef struct { cv_var vars[CV_MAX]; int n; } CondVar;
void condvar_init(CondVar *c) { if(c) memset(c,0,sizeof(*c)); }
int condvar_set(CondVar *c, const char *name, const char *value) {
    if(!c||!name) return -1;
    for (int i=0;i<c->n;i++) if (strcmp(c->vars[i].name,name)==0) { snprintf(c->vars[i].value,CV_VAL,"%s",value?value:""); return 0; }
    if (c->n>=CV_MAX) return -1;
    snprintf(c->vars[c->n].name,CV_NAME,"%s",name); snprintf(c->vars[c->n].value,CV_VAL,"%s",value?value:""); c->n++; return 0;
}
const char *condvar_get(const CondVar *c, const char *name) {
    if(!c||!name) return 0;
    for (int i=0;i<c->n;i++) if (strcmp(c->vars[i].name,name)==0) return c->vars[i].value;
    return 0;
}
/* evaluate simple condition: "VAR==value", "VAR!=value", "VAR" (truthy) */
int condvar_eval(const CondVar *c, const char *cond) {
    if(!c||!cond) return 0;
    const char *eq=strstr(cond,"==");
    if (eq) { char name[CV_NAME]; int nl=(int)(eq-cond); if(nl>=CV_NAME)nl=CV_NAME-1; memcpy(name,cond,nl); name[nl]=0;
        const char *v=condvar_get(c,name); return v && strcmp(v,eq+2)==0; }
    const char *ne=strstr(cond,"!=");
    if (ne) { char name[CV_NAME]; int nl=(int)(ne-cond); if(nl>=CV_NAME)nl=CV_NAME-1; memcpy(name,cond,nl); name[nl]=0;
        const char *v=condvar_get(c,name); return !v || strcmp(v,ne+2)!=0; }
    const char *v=condvar_get(c,cond);
    return v && *v && strcmp(v,"0")!=0 && strcmp(v,"false")!=0;
}
int condvar_count(const CondVar *c) { return c?c->n:-1; }
