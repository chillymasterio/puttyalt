#include <string.h>
#include <stdio.h>
int unicode_is_wide(unsigned int cp) {
    /* CJK Unified Ideographs and common wide ranges */
    if (cp >= 0x1100 && cp <= 0x115F) return 1;
    if (cp >= 0x2E80 && cp <= 0x9FFF) return 1;
    if (cp >= 0xAC00 && cp <= 0xD7AF) return 1;
    if (cp >= 0xF900 && cp <= 0xFAFF) return 1;
    if (cp >= 0xFE10 && cp <= 0xFE6F) return 1;
    if (cp >= 0xFF01 && cp <= 0xFF60) return 1;
    if (cp >= 0x20000 && cp <= 0x2FFFF) return 1;
    return 0;
}
int unicode_utf8_len(unsigned char c) {
    if (c < 0x80) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}
unsigned int unicode_decode_utf8(const unsigned char *buf, int len, int *consumed) {
    if (len <= 0) { *consumed = 0; return 0; }
    unsigned char c = buf[0];
    if (c < 0x80) { *consumed = 1; return c; }
    if ((c&0xE0)==0xC0 && len>=2) { *consumed=2; return ((c&0x1F)<<6)|(buf[1]&0x3F); }
    if ((c&0xF0)==0xE0 && len>=3) { *consumed=3; return ((c&0x0F)<<12)|((buf[1]&0x3F)<<6)|(buf[2]&0x3F); }
    if ((c&0xF8)==0xF0 && len>=4) { *consumed=4; return ((c&0x07)<<18)|((buf[1]&0x3F)<<12)|((buf[2]&0x3F)<<6)|(buf[3]&0x3F); }
    *consumed = 1; return 0xFFFD;
}
int unicode_display_width(const char *str) {
    int w = 0; const unsigned char *p = (const unsigned char*)str;
    while (*p) { int consumed; unsigned int cp = unicode_decode_utf8(p, 4, &consumed); w += unicode_is_wide(cp) ? 2 : 1; p += consumed; }
    return w;
}
