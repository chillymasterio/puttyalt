/* puttyalt_jsonpatch.c - JSON merge patch application (RFC 7386 lite). */
#include <string.h>
#include <stdio.h>
#define JP_MAX 32
#define JP_KEY 40
#define JP_VAL 96
typedef struct { char key[JP_KEY]; char value[JP_VAL]; int deleted; } jp_field;
typedef struct { jp_field fields[JP_MAX]; int n; } JsonPatch;
void jsonpatch_init(JsonPatch *j) { if(j) memset(j,0,sizeof(*j)); }
int jsonpatch_set(JsonPatch *j, const char *key, const char *value) {
    if(!j||!key) return -1;
    for (int i=0;i<j->n;i++) if (strcmp(j->fields[i].key,key)==0) { snprintf(j->fields[i].value,JP_VAL,"%s",value?value:""); j->fields[i].deleted=(value==0); return 0; }
    if (j->n>=JP_MAX) return -1;
    snprintf(j->fields[j->n].key,JP_KEY,"%s",key); snprintf(j->fields[j->n].value,JP_VAL,"%s",value?value:""); j->fields[j->n].deleted=(value==0); j->n++;
    return 0;
}
int jsonpatch_delete(JsonPatch *j, const char *key) {
    if(!j||!key) return -1;
    return jsonpatch_set(j,key,0);
}
int jsonpatch_apply(const JsonPatch *j, char *out, int outlen) {
    if(!j||!out) return -1;
    int pos=0; out[pos++]='{'; int first=1;
    for (int i=0;i<j->n && pos<outlen-2;i++) {
        if (j->fields[i].deleted) continue;
        if (!first) out[pos++]=',';
        pos+=snprintf(out+pos,outlen-pos,"\"%s\":\"%s\"",j->fields[i].key,j->fields[i].value); first=0;
    }
    out[pos++]='}'; out[pos]=0; return pos;
}
int jsonpatch_count(const JsonPatch *j) { return j?j->n:-1; }
