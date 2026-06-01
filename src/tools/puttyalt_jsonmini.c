/* puttyalt_jsonmini.c - Minimal flat JSON object value extractor (no nesting). */
#include <string.h>
#include <stdio.h>
int jsonmini_get_string(const char *json, const char *key, char *out, int outlen) {
    if(!json||!key||!out) return -1;
    char pat[80]; snprintf(pat,sizeof(pat),"\"%s\"",key);
    const char *k=strstr(json,pat); if(!k) return -1;
    const char *c=strchr(k+strlen(pat),':'); if(!c) return -1;
    const char *q=strchr(c,'"'); if(!q) return -1;
    q++; int o=0;
    while (*q && *q!='"' && o<outlen-1) { if (*q=='\\'&&q[1]) q++; out[o++]=*q++; }
    out[o]=0; return o;
}
int jsonmini_get_int(const char *json, const char *key, int fallback) {
    if(!json||!key) return fallback;
    char pat[80]; snprintf(pat,sizeof(pat),"\"%s\"",key);
    const char *k=strstr(json,pat); if(!k) return fallback;
    const char *c=strchr(k+strlen(pat),':'); if(!c) return fallback;
    c++; while (*c==' '||*c=='\t') c++;
    int neg=0; if (*c=='-'){neg=1;c++;}
    if (*c<'0'||*c>'9') return fallback;
    int v=0; while (*c>='0'&&*c<='9') v=v*10+(*c++-'0');
    return neg?-v:v;
}
int jsonmini_has_key(const char *json, const char *key) {
    if(!json||!key) return 0;
    char pat[80]; snprintf(pat,sizeof(pat),"\"%s\"",key);
    return strstr(json,pat)!=NULL?1:0;
}
