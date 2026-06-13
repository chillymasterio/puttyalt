/* puttyalt_inifile.c - INI file parser with sections. */
#include <string.h>
#include <stdio.h>
#define IF_MAX 96
#define IF_SEC 32
#define IF_KEY 40
#define IF_VAL 128
typedef struct { char section[IF_SEC]; char key[IF_KEY]; char value[IF_VAL]; } if_entry;
typedef struct { if_entry e[IF_MAX]; int n; } IniFile;
void inifile_init(IniFile *f) { if(f) memset(f,0,sizeof(*f)); }
int inifile_parse(IniFile *f, const char *text) {
    if(!f||!text) return -1;
    f->n=0;
    char section[IF_SEC]="default"; const char *line=text;
    while (*line && f->n<IF_MAX) {
        const char *nl=strchr(line,'\n'); int ll=nl?(int)(nl-line):(int)strlen(line);
        const char *p=line; while(*p==' '||*p=='\t')p++;
        if (ll>0 && *p=='[') {
            const char *end=strchr(p,']');
            if (end) { int sl=(int)(end-p-1); if(sl>=IF_SEC)sl=IF_SEC-1; memcpy(section,p+1,sl); section[sl]=0; }
        } else if (ll>0 && *p!='#' && *p!=';') {
            const char *eq=memchr(p,'=',ll-(p-line));
            if (eq) {
                if_entry *e=&f->e[f->n++]; snprintf(e->section,IF_SEC,"%s",section);
                int kl=(int)(eq-p); if(kl>=IF_KEY)kl=IF_KEY-1; memcpy(e->key,p,kl); e->key[kl]=0;
                /* trim key */ while(kl>0&&e->key[kl-1]==' ')e->key[--kl]=0;
                const char *v=eq+1; while(*v==' ')v++;
                int vl=(int)(line+ll-v); if(vl>=IF_VAL)vl=IF_VAL-1; if(vl<0)vl=0; memcpy(e->value,v,vl); e->value[vl]=0;
                while(vl>0&&(e->value[vl-1]==' '||e->value[vl-1]=='\r'))e->value[--vl]=0;
            }
        }
        if(!nl) break;
        line=nl+1;
    }
    return f->n;
}
const char *inifile_get(const IniFile *f, const char *section, const char *key) {
    if(!f||!section||!key) return 0;
    for (int i=0;i<f->n;i++) if (strcmp(f->e[i].section,section)==0 && strcmp(f->e[i].key,key)==0) return f->e[i].value;
    return 0;
}
int inifile_count(const IniFile *f) { return f?f->n:-1; }
