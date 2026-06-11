/* puttyalt_base32.c - Base32 encode/decode (RFC 4648, for TOTP secrets). */
#include <string.h>
static const char B32[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
int base32_encode(const unsigned char *in, int len, char *out, int outlen) {
    if(!in||!out) return -1; int o=0; int bits=0; unsigned long buf=0;
    for (int i=0;i<len;i++) {
        buf=(buf<<8)|in[i]; bits+=8;
        while (bits>=5 && o<outlen-1) { out[o++]=B32[(buf>>(bits-5))&0x1F]; bits-=5; }
    }
    if (bits>0 && o<outlen-1) out[o++]=B32[(buf<<(5-bits))&0x1F];
    while (o%8!=0 && o<outlen-1) out[o++]='=';
    out[o]=0; return o;
}
static int b32_val(char c) {
    if (c>='A'&&c<='Z') return c-'A'; if (c>='a'&&c<='z') return c-'a';
    if (c>='2'&&c<='7') return c-'2'+26; return -1;
}
int base32_decode(const char *in, unsigned char *out, int outlen) {
    if(!in||!out) return -1; int o=0; int bits=0; unsigned long buf=0;
    for (const char *p=in; *p; p++) {
        if (*p=='=') break; int v=b32_val(*p); if (v<0) continue;
        buf=(buf<<5)|v; bits+=5;
        if (bits>=8 && o<outlen) { out[o++]=(buf>>(bits-8))&0xFF; bits-=8; }
    }
    return o;
}
