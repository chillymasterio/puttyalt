/* puttyalt_sesstemplate.c - Session templates with field locking + instantiation. */
#include <string.h>
#include <stdio.h>
#define ST_MAX 24
#define ST_NAME 48
#define ST_VAL 128
typedef struct { char key[ST_NAME]; char val[ST_VAL]; int locked; } st_field;
typedef struct { char name[ST_NAME]; st_field f[ST_MAX]; int nf; } SessTemplate;
void sesstemplate_init(SessTemplate *t, const char *name) {
    if(!t) return;
    memset(t,0,sizeof(*t)); snprintf(t->name,ST_NAME,"%s",name?name:"");
}
int sesstemplate_set(SessTemplate *t, const char *key, const char *val, int locked) {
    if(!t||!key) return -1;
    for (int i=0;i<t->nf;i++) if (strcmp(t->f[i].key,key)==0) {
        if (t->f[i].locked) return 1;
        /* cannot override locked */
        snprintf(t->f[i].val,ST_VAL,"%s",val?val:""); t->f[i].locked=locked?1:0; return 0;
    }
    if (t->nf>=ST_MAX) return -1;
    st_field *f=&t->f[t->nf++]; snprintf(f->key,ST_NAME,"%s",key); snprintf(f->val,ST_VAL,"%s",val?val:""); f->locked=locked?1:0; return 0;
}
int sesstemplate_instantiate(const SessTemplate *t, char *buf, int buflen) {
    if(!t||!buf) return -1;
    int pos=snprintf(buf,0,"%s","");
    pos=0;
    for (int i=0;i<t->nf && pos<buflen;i++) pos+=snprintf(buf+pos,buflen-pos,"%s=%s%s\n",t->f[i].key,t->f[i].val,t->f[i].locked?" #locked":"");
    return pos;
}
int sesstemplate_field_count(const SessTemplate *t) { return t?t->nf:-1; }
