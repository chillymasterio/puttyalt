/* puttyalt_settingui.c - Settings UI model: categories, fields, search. */
#include <string.h>
#include <stdio.h>
#define SU_MAX 96
#define SU_NAME 48
#define SU_CAT 24
enum su_type { SU_BOOL=0, SU_INT=1, SU_STRING=2, SU_ENUM=3, SU_COLOR=4 };
typedef struct { char name[SU_NAME]; char category[SU_CAT]; int type; char value[64]; int modified; } su_field;
typedef struct { su_field f[SU_MAX]; int n; } SettingUI;
void settingui_init(SettingUI *s) { if(s) memset(s,0,sizeof(*s)); }
int settingui_add(SettingUI *s, const char *name, const char *category, int type, const char *def) {
    if(!s||s->n>=SU_MAX||!name) return -1;
    su_field *f=&s->f[s->n++]; snprintf(f->name,SU_NAME,"%s",name); snprintf(f->category,SU_CAT,"%s",category?category:"General");
    f->type=type; snprintf(f->value,64,"%s",def?def:""); return 0;
}
int settingui_set(SettingUI *s, const char *name, const char *value) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->f[i].name,name)==0) { snprintf(s->f[i].value,64,"%s",value?value:""); s->f[i].modified=1; return 0; }
    return -1;
}
int settingui_by_category(const SettingUI *s, const char *cat, int *out_idx, int cap) {
    if(!s||!cat) return -1;
    int n=0;
    for (int i=0;i<s->n && n<cap;i++) if (strcmp(s->f[i].category,cat)==0) out_idx[n++]=i;
    return n;
}
int settingui_search(const SettingUI *s, const char *query, int *out_idx, int cap) {
    if(!s||!query) return -1;
    int n=0;
    for (int i=0;i<s->n && n<cap;i++) if (strstr(s->f[i].name,query)) out_idx[n++]=i;
    return n;
}
int settingui_modified_count(const SettingUI *s) {
    if(!s) return -1;
    int n=0; for(int i=0;i<s->n;i++) if(s->f[i].modified)n++; return n;
}
