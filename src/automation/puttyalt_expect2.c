/* puttyalt_expect2.c - Expect engine v2 with regex-lite patterns + send queue. */
#include <string.h>
#include <stdio.h>
#define EX_MAX 16
#define EX_PAT 96
#define EX_SEND 160
typedef struct { char pattern[EX_PAT]; char send[EX_SEND]; int matched; } ex_rule;
typedef struct { ex_rule rules[EX_MAX]; int n; char buffer[1024]; int buflen; int timeout_ms; } Expect2;
void expect2_init(Expect2 *e, int timeout_ms) {
    if(!e) return;
    memset(e,0,sizeof(*e)); e->timeout_ms=timeout_ms>0?timeout_ms:10000;
}
int expect2_add(Expect2 *e, const char *pattern, const char *send) {
    if(!e||e->n>=EX_MAX||!pattern) return -1;
    ex_rule *r=&e->rules[e->n++]; snprintf(r->pattern,EX_PAT,"%s",pattern); snprintf(r->send,EX_SEND,"%s",send?send:"");
    return e->n-1;
}
int expect2_feed(Expect2 *e, const char *data, char *out_send, int outlen) {
    if(!e||!data) return -1;
    int dl=(int)strlen(data);
    if (e->buflen+dl >= 1024) { /* slide buffer */ int keep=512; memmove(e->buffer,e->buffer+e->buflen-keep,keep); e->buflen=keep; }
    memcpy(e->buffer+e->buflen,data,dl); e->buflen+=dl; e->buffer[e->buflen]=0;
    for (int i=0;i<e->n;i++) if (!e->rules[i].matched && strstr(e->buffer,e->rules[i].pattern)) {
        e->rules[i].matched=1;
        if (out_send) snprintf(out_send,outlen,"%s",e->rules[i].send);
        e->buflen=0; e->buffer[0]=0; /* consume buffer on match */
        return i;
    }
    return -1;
}
int expect2_all_matched(const Expect2 *e) {
    if(!e) return 0;
    for(int i=0;i<e->n;i++) if(!e->rules[i].matched) return 0;
    return 1;
}
void expect2_reset(Expect2 *e) { if(e){ e->buflen=0; e->buffer[0]=0; for(int i=0;i<e->n;i++) e->rules[i].matched=0; } }
int expect2_rule_count(const Expect2 *e) { return e?e->n:-1; }
