/* puttyalt_tlsverify.c - TLS certificate chain verification state. */
#include <string.h>
#include <stdio.h>
#define TV_CHAIN 8
#define TV_NAME 128
typedef struct {
    char subject[TV_NAME];
    char issuer[TV_NAME];
    int self_signed;
    int expired;
    int trusted_root;
} tv_cert;
typedef struct {
    tv_cert chain[TV_CHAIN];
    int depth;
    int verified;
} TlsVerify;
void tlsverify_init(TlsVerify *t) {
    if (t) memset(t, 0, sizeof(*t));
}
int tlsverify_add_cert(TlsVerify *t, const char *subject, const char *issuer, int expired, int trusted_root) {
    if (!t || t->depth >= TV_CHAIN) return -1;
    tv_cert *c = &t->chain[t->depth];
    snprintf(c->subject, TV_NAME, "%s", subject ? subject : "");
    snprintf(c->issuer, TV_NAME, "%s", issuer ? issuer : "");
    c->self_signed = (subject && issuer && strcmp(subject, issuer) == 0);
    c->expired = expired ? 1 : 0;
    c->trusted_root = trusted_root ? 1 : 0;
    return t->depth++;
}
int tlsverify_validate(TlsVerify *t) {
    if (!t || t->depth == 0) return -1;
    for (int i = 0; i < t->depth; i++) {
        if (t->chain[i].expired) {
            t->verified = 0;
            return -1;
        }
    }
    int has_root = 0;
    for (int i = 0; i < t->depth; i++)
        if (t->chain[i].trusted_root) has_root = 1;
    t->verified = has_root;
    return has_root ? 0 : -1;
}
int tlsverify_is_verified(const TlsVerify *t) {
    return t ? t->verified : -1;
}
int tlsverify_depth(const TlsVerify *t) {
    return t ? t->depth : -1;
}
