/* puttyalt_aicache.c - Cache of AI completions keyed by prompt hash with TTL. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define AC_MAX 32
#define AC_VAL 512
typedef struct { uint64_t hash; char val[AC_VAL]; uint64_t ts; int hits; } ac_entry;
typedef struct { ac_entry e[AC_MAX]; int n; int ttl_ms; } AiCache;
static uint64_t ac_hash(const char *s) { uint64_t h=1469598103934665603ULL; for(;*s;s++){h^=(uint8_t)*s;h*=1099511628211ULL;} return h; }
void aicache_init(AiCache *c, int ttl_ms) { if(!c) return; memset(c,0,sizeof(*c)); c->ttl_ms=ttl_ms>0?ttl_ms:300000; }
int aicache_put(AiCache *c, const char *prompt, const char *completion, uint64_t now_ms) {
    if(!c||!prompt||!completion) return -1;
    uint64_t h=ac_hash(prompt);
    for (int i=0;i<c->n;i++) if (c->e[i].hash==h) { snprintf(c->e[i].val,AC_VAL,"%s",completion); c->e[i].ts=now_ms; return 0; }
    if (c->n>=AC_MAX) { /* evict oldest */ int o=0; for(int i=1;i<c->n;i++) if(c->e[i].ts<c->e[o].ts)o=i; c->e[o]=c->e[c->n-1]; c->n--; }
    ac_entry *e=&c->e[c->n++]; e->hash=h; snprintf(e->val,AC_VAL,"%s",completion); e->ts=now_ms; return 0;
}
int aicache_get(AiCache *c, const char *prompt, uint64_t now_ms, char *out, int outlen) {
    if(!c||!prompt) return -1; uint64_t h=ac_hash(prompt);
    for (int i=0;i<c->n;i++) if (c->e[i].hash==h) {
        if ((now_ms-c->e[i].ts)>(uint64_t)c->ttl_ms) return -1;
        c->e[i].hits++; if(out) snprintf(out,outlen,"%s",c->e[i].val); return 0;
    }
    return -1;
}
int aicache_count(const AiCache *c) { return c?c->n:-1; }
