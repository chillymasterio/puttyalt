/* puttyalt_fido2.c - FIDO2/U2F security key authentication state. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
enum f2_state { F2_IDLE=0, F2_WAITING_TOUCH=1, F2_VERIFIED=2, F2_TIMEOUT=3, F2_ERROR=4 };
typedef struct { int state; char rp_id[96]; int requires_pin; int user_verified; uint64_t challenge_ms; int timeout_ms; } Fido2;
void fido2_init(Fido2 *f, const char *rp_id, int timeout_ms) {
    if(!f) return; memset(f,0,sizeof(*f)); snprintf(f->rp_id,96,"%s",rp_id?rp_id:"ssh:"); f->timeout_ms=timeout_ms>0?timeout_ms:30000;
}
int fido2_begin(Fido2 *f, int requires_pin, uint64_t now_ms) {
    if(!f) return -1; f->state=F2_WAITING_TOUCH; f->requires_pin=requires_pin?1:0; f->challenge_ms=now_ms; return 0;
}
int fido2_touch(Fido2 *f, int pin_ok, uint64_t now_ms) {
    if(!f||f->state!=F2_WAITING_TOUCH) return -1;
    if ((now_ms-f->challenge_ms)>(uint64_t)f->timeout_ms) { f->state=F2_TIMEOUT; return -1; }
    if (f->requires_pin && !pin_ok) { f->state=F2_ERROR; return -1; }
    f->state=F2_VERIFIED; f->user_verified=1; return 0;
}
int fido2_state(const Fido2 *f) { return f?f->state:-1; }
int fido2_is_verified(const Fido2 *f) { return (f&&f->state==F2_VERIFIED)?1:0; }
