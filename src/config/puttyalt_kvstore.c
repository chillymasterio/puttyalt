/* puttyalt_kvstore.c - Tiny in-memory key/value config store with typed getters. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define KV_MAX 128
#define KV_KEY 64
#define KV_VAL 256
typedef struct { char k[KV_KEY]; char v[KV_VAL]; } kv_pair;
typedef struct { kv_pair p[KV_MAX]; int n; } KvStore;
void kvstore_init(KvStore *s) { if(s) memset(s,0,sizeof(*s)); }
int kvstore_set(KvStore *s, const char *key, const char *val) {
    if(!s||!key) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->p[i].k,key)==0) { snprintf(s->p[i].v,KV_VAL,"%s",val?val:""); return 0; }
    if (s->n>=KV_MAX) return -1;
    snprintf(s->p[s->n].k,KV_KEY,"%s",key); snprintf(s->p[s->n].v,KV_VAL,"%s",val?val:""); s->n++; return 0;
}
const char *kvstore_get(const KvStore *s, const char *key, const char *fallback) {
    if(!s||!key) return fallback;
    for (int i=0;i<s->n;i++) if (strcmp(s->p[i].k,key)==0) return s->p[i].v;
    return fallback;
}
int kvstore_get_int(const KvStore *s, const char *key, int fallback) {
    const char *v=kvstore_get(s,key,NULL); return v?atoi(v):fallback;
}
int kvstore_get_bool(const KvStore *s, const char *key, int fallback) {
    const char *v=kvstore_get(s,key,NULL);
    if(!v) return fallback;
    return (strcmp(v,"1")==0||strcmp(v,"true")==0||strcmp(v,"yes")==0)?1:0;
}
int kvstore_remove(KvStore *s, const char *key) {
    if(!s||!key) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->p[i].k,key)==0) {
        memmove(&s->p[i],&s->p[i+1],sizeof(kv_pair)*(s->n-i-1)); s->n--; return 0;
    }
    return -1;
}
int kvstore_serialize(const KvStore *s, char *buf, int buflen) {
    if(!s||!buf) return -1; int pos=0;
    for (int i=0;i<s->n && pos<buflen;i++) pos+=snprintf(buf+pos,buflen-pos,"%s=%s\n",s->p[i].k,s->p[i].v);
    return pos;
}
int kvstore_count(const KvStore *s) { return s?s->n:-1; }
