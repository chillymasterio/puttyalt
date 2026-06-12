/* puttyalt_export3.c - Settings export to portable archive format. */
#include <string.h>
#include <stdio.h>
#define EX_MAX 64
#define EX_KEY 48
#define EX_VAL 128
typedef struct { char section[32]; char key[EX_KEY]; char value[EX_VAL]; } ex_item;
typedef struct { ex_item items[EX_MAX]; int n; int version; } Export3;
void export3_init(Export3 *e, int version) { if(e){ memset(e,0,sizeof(*e)); e->version=version; } }
int export3_add(Export3 *e, const char *section, const char *key, const char *value) {
    if(!e||e->n>=EX_MAX) return -1;
    ex_item *it=&e->items[e->n++]; snprintf(it->section,32,"%s",section?section:"general");
    snprintf(it->key,EX_KEY,"%s",key?key:""); snprintf(it->value,EX_VAL,"%s",value?value:""); return 0;
}
int export3_serialize(const Export3 *e, char *out, int outlen) {
    if(!e||!out) return -1;
    int pos=0;
    pos+=snprintf(out+pos,outlen-pos,"# PuttyAlt Export v%d\n",e->version);
    char cur[32]="";
    for (int i=0;i<e->n && pos<outlen;i++) {
        if (strcmp(cur,e->items[i].section)!=0) { snprintf(cur,32,"%s",e->items[i].section); pos+=snprintf(out+pos,outlen-pos,"[%s]\n",cur); }
        pos+=snprintf(out+pos,outlen-pos,"%s=%s\n",e->items[i].key,e->items[i].value);
    }
    return pos;
}
int export3_count(const Export3 *e) { return e?e->n:-1; }
