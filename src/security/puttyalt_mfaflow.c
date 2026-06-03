/* puttyalt_mfaflow.c - Multi-factor auth flow state machine (push/totp/webauthn). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
enum mfa_method { MFA_NONE=0, MFA_TOTP=1, MFA_PUSH=2, MFA_WEBAUTHN=3, MFA_BACKUP=4 };
enum mfa_state { MFA_IDLE=0, MFA_CHALLENGED, MFA_VERIFYING, MFA_PASSED, MFA_FAILED, MFA_EXPIRED };
typedef struct { int method; int state; int attempts; int max_attempts; uint64_t challenge_ms; int timeout_ms; } MfaFlow;
void mfaflow_init(MfaFlow *m, int timeout_ms) {
    if(!m) return; memset(m,0,sizeof(*m)); m->max_attempts=3; m->timeout_ms=timeout_ms>0?timeout_ms:120000;
}
int mfaflow_challenge(MfaFlow *m, int method, uint64_t now_ms) {
    if(!m||m->state==MFA_VERIFYING) return -1;
    m->method=method; m->state=MFA_CHALLENGED; m->challenge_ms=now_ms; m->attempts=0; return 0;
}
int mfaflow_verify(MfaFlow *m, int success, uint64_t now_ms) {
    if(!m||m->state!=MFA_CHALLENGED) return -1;
    if ((now_ms-m->challenge_ms)>(uint64_t)m->timeout_ms) { m->state=MFA_EXPIRED; return -1; }
    m->attempts++;
    if (success) { m->state=MFA_PASSED; return 0; }
    if (m->attempts>=m->max_attempts) { m->state=MFA_FAILED; return -1; }
    return 1; /* retry allowed */
}
int mfaflow_state(const MfaFlow *m) { return m?m->state:-1; }
int mfaflow_is_passed(const MfaFlow *m) { return (m&&m->state==MFA_PASSED)?1:0; }
