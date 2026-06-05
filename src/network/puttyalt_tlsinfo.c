/* puttyalt_tlsinfo.c - TLS connection info parser (version, cipher, ALPN). */
#include <string.h>
#include <stdio.h>
typedef struct { char version[16]; char cipher[64]; char alpn[24]; int bits; int forward_secret; } TlsInfo;
int tlsinfo_parse_version(const char *v, TlsInfo *out) {
    if(!v||!out) return -1;
    snprintf(out->version,16,"%s",v);
    return 0;
}
int tlsinfo_set_cipher(TlsInfo *t, const char *cipher) {
    if(!t||!cipher) return -1;
    snprintf(t->cipher,64,"%s",cipher);
    t->forward_secret = (strstr(cipher,"ECDHE")||strstr(cipher,"DHE"))?1:0;
    if (strstr(cipher,"256")) t->bits=256; else if (strstr(cipher,"128")) t->bits=128; else t->bits=0;
    return 0;
}
int tlsinfo_is_secure(const TlsInfo *t) {
    if(!t) return 0;
    int modern = strcmp(t->version,"TLS1.3")==0 || strcmp(t->version,"TLS1.2")==0;
    return (modern && t->forward_secret && t->bits>=128)?1:0;
}
int tlsinfo_summary(const TlsInfo *t, char *buf, int buflen) {
    if(!t||!buf) return -1;
    return snprintf(buf,buflen,"%s %s %d-bit%s",t->version,t->cipher,t->bits,t->forward_secret?" (PFS)":"");
}
