/* puttyalt_sessvault.c - Per-session credential vault binding with TTL unlock. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SV_MAX 32
#define SV_NAME 48
typedef struct { char session[SV_NAME]; int vault_key_id; uint64_t unlocked_until; int locked; } sv_binding;
typedef struct { sv_binding b[SV_MAX]; int n; int default_ttl_ms; } SessVault;
void sessvault_init(SessVault *s, int default_ttl_ms) {
    if(!s) return; memset(s,0,sizeof(*s)); s->default_ttl_ms=default_ttl_ms>0?default_ttl_ms:900000;
}
int sessvault_bind(SessVault *s, const char *session, int vault_key_id) {
    if(!s||s->n>=SV_MAX||!session) return -1;
    sv_binding *b=&s->b[s->n++]; snprintf(b->session,SV_NAME,"%s",session); b->vault_key_id=vault_key_id; b->locked=1;
    return 0;
}
int sessvault_unlock(SessVault *s, const char *session, uint64_t now_ms) {
    if(!s||!session) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->b[i].session,session)==0) {
        s->b[i].locked=0; s->b[i].unlocked_until=now_ms+s->default_ttl_ms; return 0;
    }
    return -1;
}
int sessvault_is_unlocked(SessVault *s, const char *session, uint64_t now_ms) {
    if(!s||!session) return 0;
    for (int i=0;i<s->n;i++) if (strcmp(s->b[i].session,session)==0) {
        if (!s->b[i].locked && now_ms>=s->b[i].unlocked_until) s->b[i].locked=1;
        return !s->b[i].locked;
    }
    return 0;
}
int sessvault_count(const SessVault *s) { return s?s->n:-1; }
