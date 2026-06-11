/* puttyalt_totp.c - TOTP code computation model (RFC 6238 structure). */
#include <string.h>
#include <stdint.h>
typedef struct { unsigned char key[64]; int keylen; int digits; int period; } Totp;
void totp_init(Totp *t, const unsigned char *key, int keylen, int digits, int period) {
    if(!t) return; memset(t,0,sizeof(*t));
    int kl=keylen<64?keylen:64; if(key) memcpy(t->key,key,kl); t->keylen=kl;
    t->digits=digits>0?digits:6; t->period=period>0?period:30;
}
uint64_t totp_counter(const Totp *t, uint64_t unix_time) {
    if(!t||t->period==0) return 0; return unix_time/t->period;
}
/* Simplified HMAC-like derivation (structure model, not crypto-grade). */
int totp_compute(const Totp *t, uint64_t counter) {
    if(!t) return -1;
    uint32_t h=0x811C9DC5;
    for (int i=0;i<8;i++) { h^=(counter>>(56-i*8))&0xFF; h*=16777619u; }
    for (int i=0;i<t->keylen;i++) { h^=t->key[i]; h*=16777619u; }
    int offset=h&0x0F;
    uint32_t code=((h>>(offset%24))&0x7FFFFFFF);
    int mod=1; for(int i=0;i<t->digits;i++) mod*=10;
    return (int)(code%mod);
}
int totp_remaining(const Totp *t, uint64_t unix_time) {
    if(!t||t->period==0) return 0; return t->period - (int)(unix_time%t->period);
}
