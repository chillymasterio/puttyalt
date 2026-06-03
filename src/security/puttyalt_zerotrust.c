/* puttyalt_zerotrust.c - Zero-trust policy engine: per-action allow/deny rules. */
#include <string.h>
#include <stdio.h>
#define ZT_MAX 48
#define ZT_NAME 64
enum zt_decision { ZT_DENY=0, ZT_ALLOW=1, ZT_PROMPT=2 };
typedef struct { char resource[ZT_NAME]; char action[ZT_NAME]; int decision; int require_mfa; } zt_rule;
typedef struct { zt_rule r[ZT_MAX]; int n; int default_decision; } ZeroTrust;
void zerotrust_init(ZeroTrust *z, int default_decision) {
    if(!z) return; memset(z,0,sizeof(*z)); z->default_decision=default_decision;
}
int zerotrust_add_rule(ZeroTrust *z, const char *resource, const char *action, int decision, int require_mfa) {
    if(!z||z->n>=ZT_MAX||!resource||!action) return -1;
    zt_rule *r=&z->r[z->n++]; snprintf(r->resource,ZT_NAME,"%s",resource);
    snprintf(r->action,ZT_NAME,"%s",action); r->decision=decision; r->require_mfa=require_mfa?1:0; return 0;
}
static int zt_match(const char *pat, const char *s) {
    if (strcmp(pat,"*")==0) return 1;
    int pl=(int)strlen(pat);
    if (pl>0 && pat[pl-1]=='*') return strncmp(pat,s,pl-1)==0;
    return strcmp(pat,s)==0;
}
int zerotrust_evaluate(const ZeroTrust *z, const char *resource, const char *action, int *out_mfa) {
    if(!z||!resource||!action) return ZT_DENY;
    for (int i=0;i<z->n;i++) if (zt_match(z->r[i].resource,resource) && zt_match(z->r[i].action,action)) {
        if (out_mfa) *out_mfa=z->r[i].require_mfa;
        return z->r[i].decision;
    }
    if (out_mfa) *out_mfa=0;
    return z->default_decision;
}
int zerotrust_rule_count(const ZeroTrust *z) { return z?z->n:-1; }
