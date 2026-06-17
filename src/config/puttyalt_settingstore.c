/* puttyalt_settingstore.c - Persistent settings store with change tracking. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SS_MAX 128
#define SS_KEY 48
#define SS_VAL 128
typedef struct { char key[SS_KEY]; char value[SS_VAL]; int dirty; } ss_setting;
typedef struct { ss_setting s[SS_MAX]; int n; int dirty_count; } SettingStore;
void settingstore_init(SettingStore *s) { if(s) memset(s,0,sizeof(*s)); }
int settingstore_set(SettingStore *s, const char *key, const char *value) {
    if(!s||!key) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->s[i].key,key)==0) {
        if (strcmp(s->s[i].value,value?value:"")!=0) { snprintf(s->s[i].value,SS_VAL,"%s",value?value:""); if(!s->s[i].dirty){s->s[i].dirty=1;s->dirty_count++;} }
        return 0;
    }
    if (s->n>=SS_MAX) return -1;
    ss_setting *st=&s->s[s->n++]; snprintf(st->key,SS_KEY,"%s",key); snprintf(st->value,SS_VAL,"%s",value?value:""); st->dirty=1; s->dirty_count++;
    return 0;
}
const char *settingstore_get(const SettingStore *s, const char *key, const char *def) {
    if(!s||!key) return def;
    for (int i=0;i<s->n;i++) if (strcmp(s->s[i].key,key)==0) return s->s[i].value;
    return def;
}
int settingstore_get_int(const SettingStore *s, const char *key, int def) {
    const char *v=settingstore_get(s,key,0); return v?atoi(v):def;
}
int settingstore_serialize(const SettingStore *s, char *out, int outlen) {
    if(!s||!out) return -1;
    int pos=0;
    for (int i=0;i<s->n && pos<outlen;i++) pos+=snprintf(out+pos,outlen-pos,"%s=%s\n",s->s[i].key,s->s[i].value);
    return pos;
}
void settingstore_mark_saved(SettingStore *s) {
    if(!s) return;
    for(int i=0;i<s->n;i++) s->s[i].dirty=0;
    s->dirty_count=0;
}
int settingstore_dirty_count(const SettingStore *s) { return s?s->dirty_count:-1; }
