/* puttyalt_jsonpath.c - Simple JSONPath-lite extractor (dot notation, flat). */
#include <string.h>
#include <stdio.h>
int jsonpath_get(const char *json, const char *path, char *out, int outlen) {
    if(!json||!path||!out) return -1;
    /* path like "a.b.c" - navigate by finding each key */
    const char *cur=json; char key[64];
    const char *p=path;
    while (*p) {
        int kl=0; while (*p && *p!='.' && kl<63) key[kl++]=*p++; key[kl]=0;
        if (*p=='.') p++;
        char pat[68]; snprintf(pat,sizeof(pat),"\"%s\"",key);
        const char *found=strstr(cur,pat); if(!found) return -1;
        cur=strchr(found+strlen(pat),':'); if(!cur) return -1; cur++;
        while (*cur==' '||*cur=='\t') cur++;
    }
    /* extract value at cur */
    if (*cur=='"') { cur++; int o=0; while (*cur && *cur!='"' && o<outlen-1) out[o++]=*cur++; out[o]=0; return o; }
    int o=0; while (*cur && *cur!=',' && *cur!='}' && *cur!='\n' && o<outlen-1) out[o++]=*cur++; out[o]=0;
    /* trim trailing space */
    while (o>0 && (out[o-1]==' '||out[o-1]=='\t')) out[--o]=0;
    return o;
}
int jsonpath_exists(const char *json, const char *path) {
    char tmp[256]; return jsonpath_get(json,path,tmp,sizeof(tmp))>=0?1:0;
}
