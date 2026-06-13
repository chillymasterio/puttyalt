/* puttyalt_sessshare3.c - Session share links with scoped tokens + expiry. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SS_MAX 16
#define SS_TOKEN 40
enum ss_scope { SS_VIEW=1, SS_INPUT=2, SS_TRANSFER=4, SS_ADMIN=8 };
typedef struct { char token[SS_TOKEN]; int session_id; int scope; uint64_t expires_ms; int revoked; int uses; } ss_link;
typedef struct { ss_link links[SS_MAX]; int n; } SessShare3;
void sessshare3_init(SessShare3 *s) { if(s) memset(s,0,sizeof(*s)); }
int sessshare3_create(SessShare3 *s, const char *token, int session_id, int scope, uint64_t expires_ms) {
    if(!s||s->n>=SS_MAX||!token) return -1;
    ss_link *l=&s->links[s->n++]; snprintf(l->token,SS_TOKEN,"%s",token); l->session_id=session_id;
    l->scope=scope; l->expires_ms=expires_ms; return 0;
}
int sessshare3_validate(SessShare3 *s, const char *token, int required_scope, uint64_t now_ms) {
    if(!s||!token) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->links[i].token,token)==0) {
        if (s->links[i].revoked) return -1;
        if (s->links[i].expires_ms>0 && now_ms>=s->links[i].expires_ms) return -1;
        if ((s->links[i].scope & required_scope) != required_scope) return -2;
        /* insufficient scope */
        s->links[i].uses++; return s->links[i].session_id;
    }
    return -1;
}
int sessshare3_revoke(SessShare3 *s, const char *token) {
    if(!s||!token) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->links[i].token,token)==0) { s->links[i].revoked=1; return 0; }
    return -1;
}
int sessshare3_count(const SessShare3 *s) { return s?s->n:-1; }
