/* puttyalt_unicode2.c - UTF-8 encode/decode/validate. */
#include <stdint.h>
int unicode2_decode(const unsigned char *s, int len, uint32_t *cp, int *consumed) {
    if(!s||len<1||!cp) return -1;
    unsigned char c=s[0];
    if (c<0x80) { *cp=c; if(consumed)*consumed=1; return 0; }
    int n; uint32_t v;
    if ((c&0xE0)==0xC0) { n=2; v=c&0x1F; }
    else if ((c&0xF0)==0xE0) { n=3; v=c&0x0F; }
    else if ((c&0xF8)==0xF0) { n=4; v=c&0x07; }
    else return -1;
    if (len<n) return -1;
    for (int i=1;i<n;i++) { if((s[i]&0xC0)!=0x80) return -1; v=(v<<6)|(s[i]&0x3F); }
    *cp=v; if(consumed)*consumed=n; return 0;
}
int unicode2_encode(uint32_t cp, unsigned char *out, int outlen) {
    if(!out) return -1;
    if (cp<0x80) { if(outlen<1)return -1; out[0]=cp; return 1; }
    if (cp<0x800) { if(outlen<2)return -1; out[0]=0xC0|(cp>>6); out[1]=0x80|(cp&0x3F); return 2; }
    if (cp<0x10000) { if(outlen<3)return -1; out[0]=0xE0|(cp>>12); out[1]=0x80|((cp>>6)&0x3F); out[2]=0x80|(cp&0x3F); return 3; }
    if (cp<0x110000) { if(outlen<4)return -1; out[0]=0xF0|(cp>>18); out[1]=0x80|((cp>>12)&0x3F); out[2]=0x80|((cp>>6)&0x3F); out[3]=0x80|(cp&0x3F); return 4; }
    return -1;
}
int unicode2_validate(const unsigned char *s, int len) {
    int i=0;
    while (i<len) { uint32_t cp; int c; if(unicode2_decode(s+i,len-i,&cp,&c)<0) return 0; i+=c; }
    return 1;
}
int unicode2_strlen(const unsigned char *s, int len) {
    int i=0, count=0;
    while (i<len) { uint32_t cp; int c; if(unicode2_decode(s+i,len-i,&cp,&c)<0) { i++; continue; } i+=c; count++; }
    return count;
}
