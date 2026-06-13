/* puttyalt_schemaval.c - Config schema validation: typed field constraints. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SV_MAX 48
#define SV_NAME 40
enum sv_type { SVT_STRING=0, SVT_INT=1, SVT_BOOL=2, SVT_ENUM=3 };
typedef struct { char name[SV_NAME]; int type; int min, max; int required; } sv_field;
typedef struct { sv_field f[SV_MAX]; int n; } SchemaVal;
void schemaval_init(SchemaVal *s) { if(s) memset(s,0,sizeof(*s)); }
int schemaval_field(SchemaVal *s, const char *name, int type, int min, int max, int required) {
    if(!s||s->n>=SV_MAX||!name) return -1;
    sv_field *f=&s->f[s->n++]; snprintf(f->name,SV_NAME,"%s",name); f->type=type; f->min=min; f->max=max; f->required=required?1:0;
    return 0;
}
int schemaval_check(const SchemaVal *s, const char *name, const char *value, char *err, int errlen) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->f[i].name,name)==0) {
        const sv_field *f=&s->f[i];
        if (!value || !*value) { if (f->required) { if(err) snprintf(err,errlen,"%s is required",name); return -1; } return 0; }
        if (f->type==SVT_INT) {
            int v=atoi(value);
            if (v<f->min || v>f->max) { if(err) snprintf(err,errlen,"%s out of range [%d,%d]",name,f->min,f->max); return -1; }
        } else if (f->type==SVT_BOOL) {
            if (strcmp(value,"0")&&strcmp(value,"1")&&strcmp(value,"true")&&strcmp(value,"false")) {
                if(err) snprintf(err,errlen,"%s must be boolean",name);
                return -1;
            }
        } else if (f->type==SVT_STRING) {
            int len=(int)strlen(value);
            if (len<f->min || (f->max>0 && len>f->max)) { if(err) snprintf(err,errlen,"%s length invalid",name); return -1; }
        }
        return 0;
    }
    if(err) snprintf(err,errlen,"unknown field %s",name);
    return -1;
}
int schemaval_field_count(const SchemaVal *s) { return s?s->n:-1; }
