/* puttyalt_validate3.c - Form validation framework (field rules + errors). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define VF_MAX 24
#define VF_NAME 32
enum vf_rule { VF_REQUIRED=1, VF_NUMERIC=2, VF_RANGE=4, VF_EMAIL=8, VF_HOSTNAME=16 };
typedef struct { char name[VF_NAME]; int rules; int min, max; char error[64]; int valid; } vf_field;
typedef struct { vf_field fields[VF_MAX]; int n; } Validate3;
void validate3_init(Validate3 *v) { if(v) memset(v,0,sizeof(*v)); }
int validate3_add(Validate3 *v, const char *name, int rules, int min, int max) {
    if(!v||v->n>=VF_MAX||!name) return -1;
    vf_field *f=&v->fields[v->n]; snprintf(f->name,VF_NAME,"%s",name); f->rules=rules; f->min=min; f->max=max; f->valid=1;
    return v->n++;
}
int validate3_check(Validate3 *v, const char *name, const char *value) {
    if(!v||!name) return -1;
    for (int i=0;i<v->n;i++) if (strcmp(v->fields[i].name,name)==0) {
        vf_field *f=&v->fields[i]; f->valid=1; f->error[0]=0;
        int empty = !value||!*value;
        if ((f->rules&VF_REQUIRED) && empty) { f->valid=0; snprintf(f->error,64,"required"); return 0; }
        if (empty) return 1;
        if (f->rules&VF_NUMERIC) { for(const char*c=value;*c;c++) if(*c<'0'||*c>'9'){ f->valid=0; snprintf(f->error,64,"must be numeric"); return 0; } }
        if (f->rules&VF_RANGE) { int n=atoi(value); if(n<f->min||n>f->max){ f->valid=0; snprintf(f->error,64,"out of range [%d,%d]",f->min,f->max); return 0; } }
        if (f->rules&VF_EMAIL) { if(!strchr(value,'@')||!strchr(value,'.')){ f->valid=0; snprintf(f->error,64,"invalid email"); return 0; } }
        return 1;
    }
    return -1;
}
int validate3_all_valid(const Validate3 *v) {
    if(!v) return 0;
    for(int i=0;i<v->n;i++) if(!v->fields[i].valid) return 0;
    return 1;
}
const char *validate3_error(const Validate3 *v, const char *name) {
    if(!v||!name) return 0;
    for (int i=0;i<v->n;i++) if (strcmp(v->fields[i].name,name)==0) return v->fields[i].error;
    return 0;
}
