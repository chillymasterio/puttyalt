/* puttyalt_socks5.c - SOCKS5 handshake state machine + request builder. */
#include <string.h>
#include <stdint.h>
enum s5_state { S5_INIT=0, S5_AUTH_SENT=1, S5_AUTH_OK=2, S5_REQUEST_SENT=3, S5_CONNECTED=4, S5_FAILED=5 };
typedef struct { int state; int auth_method; int needs_auth; } Socks5;
void socks5_init(Socks5 *s, int needs_auth) { if(s){ memset(s,0,sizeof(*s)); s->needs_auth=needs_auth?1:0; } }
int socks5_build_greeting(const Socks5 *s, unsigned char *out, int outlen) {
    if(!s||!out||outlen<4) return -1;
    out[0]=0x05; /* version */
    if (s->needs_auth) { out[1]=0x02; out[2]=0x00; out[3]=0x02; return 4; }
    out[1]=0x01; out[2]=0x00; return 3;
}
int socks5_build_connect(Socks5 *s, const char *host, int port, unsigned char *out, int outlen) {
    if(!s||!host||!out) return -1;
    int hl=(int)strlen(host); if (hl>255 || outlen<7+hl) return -1;
    int p=0;
    out[p++]=0x05; out[p++]=0x01; out[p++]=0x00; out[p++]=0x03; /* domain name */
    out[p++]=(unsigned char)hl;
    memcpy(out+p,host,hl); p+=hl;
    out[p++]=(port>>8)&0xFF; out[p++]=port&0xFF;
    s->state=S5_REQUEST_SENT;
    return p;
}
int socks5_parse_reply(Socks5 *s, const unsigned char *data, int len) {
    if(!s||!data||len<2) return -1;
    if (data[0]!=0x05) { s->state=S5_FAILED; return -1; }
    if (data[1]==0x00) { s->state=S5_CONNECTED; return 0; }
    s->state=S5_FAILED; return -(int)data[1]; /* error code */
}
int socks5_state(const Socks5 *s) { return s?s->state:-1; }
