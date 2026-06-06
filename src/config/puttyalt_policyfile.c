/* puttyalt_policyfile.c - Org policy file parser (enforced/locked settings). */
#include <string.h>
#include <stdio.h>
#define PF_MAX 48
#define PF_KEY 48
#define PF_VAL 128
typedef struct { char key[PF_KEY]; char value[PF_VAL]; int locked; } pf_entry;
typedef struct { pf_entry e[PF_MAX]; int n; } PolicyFile;
void policyfile_init(PolicyFile *p) { if(p) memset(p,0,sizeof(*p)); }
int policyfile_parse(PolicyFile *p, const char *text) {
    if(!p||!text) return -1; p->n=0;
    const char *line=text;
    while (*line && p->n<PF_MAX) {
        const char *nl=strchr(line,'\n'); int linelen = nl?(int)(nl-line):(int)strlen(line);
        if (linelen>0 && line[0]!='#') {
            int locked = (line[0]=='!'); const char *start = locked?line+1:line;
            const char *eq=memchr(start,'=',linelen-(locked?1:0));
            if (eq) {
                pf_entry *e=&p->e[p->n++];
                int kl=(int)(eq-start); if(kl>=PF_KEY)kl=PF_KEY-1; memcpy(e->key,start,kl); e->key[kl]=0;
                int vl=(int)(line+linelen-(eq+1)); if(vl>=PF_VAL)vl=PF_VAL-1; if(vl<0)vl=0;
                memcpy(e->value,eq+1,vl); e->value[vl]=0; e->locked=locked;
            }
        }
        if (!nl) break; line=nl+1;
    }
    return p->n;
}
int policyfile_is_locked(const PolicyFile *p, const char *key) {
    if(!p||!key) return 0;
    for (int i=0;i<p->n;i++) if (strcmp(p->e[i].key,key)==0) return p->e[i].locked;
    return 0;
}
const char *policyfile_value(const PolicyFile *p, const char *key) {
    if(!p||!key) return 0;
    for (int i=0;i<p->n;i++) if (strcmp(p->e[i].key,key)==0) return p->e[i].value;
    return 0;
}
int policyfile_count(const PolicyFile *p) { return p?p->n:-1; }
