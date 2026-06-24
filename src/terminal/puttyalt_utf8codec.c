/* puttyalt_utf8codec.c - UTF-8 encode/decode codepoints.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int u8_encode(uint32_t cp, unsigned char *out) {
    if (cp < 0x80) { out[0] = (unsigned char)cp; return 1; }
    if (cp < 0x800) { out[0] = 0xC0|(cp>>6); out[1] = 0x80|(cp&0x3F); return 2; }
    if (cp < 0x10000) { out[0]=0xE0|(cp>>12); out[1]=0x80|((cp>>6)&0x3F); out[2]=0x80|(cp&0x3F); return 3; }
    if (cp < 0x110000) { out[0]=0xF0|(cp>>18); out[1]=0x80|((cp>>12)&0x3F); out[2]=0x80|((cp>>6)&0x3F); out[3]=0x80|(cp&0x3F); return 4; }
    return -1;
}
int u8_decode(const unsigned char *in, int n, uint32_t *cp) {
    if (n < 1) return -1;
    unsigned char c = in[0];
    if (c < 0x80) { *cp = c; return 1; }
    if ((c & 0xE0) == 0xC0) { if (n<2) return -1; *cp = ((c&0x1F)<<6)|(in[1]&0x3F); return 2; }
    if ((c & 0xF0) == 0xE0) { if (n<3) return -1; *cp = ((c&0x0F)<<12)|((in[1]&0x3F)<<6)|(in[2]&0x3F); return 3; }
    if ((c & 0xF8) == 0xF0) { if (n<4) return -1; *cp = ((c&0x07)<<18)|((in[1]&0x3F)<<12)|((in[2]&0x3F)<<6)|(in[3]&0x3F); return 4; }
    return -1;
}
int u8_strlen(const unsigned char *s, int bytes) {
    int count = 0, i = 0;
    while (i < bytes) { uint32_t cp; int adv = u8_decode(s+i, bytes-i, &cp); if (adv < 0) adv = 1; i += adv; count++; }
    return count;
}
