/* puttyalt_ansi256.c - ANSI 256-color SGR sequence parser. */
#include <stdint.h>
/* parse "38;5;N" or "38;2;R;G;B" style; returns 0 ok, sets *color, *is_fg */
int ansi256_parse(const int *params, int nparams, uint32_t *color, int *is_fg, int *consumed) {
    if(!params||nparams<1||!color) return -1;
    int p0=params[0];
    if (p0!=38 && p0!=48) return -1;
    if (is_fg) *is_fg=(p0==38);
    if (nparams>=3 && params[1]==5) {
        int idx=params[2];
        /* convert 256 index to rgb */
        if (idx<16) { static const uint32_t b[16]={0,0x800000,0x008000,0x808000,0x000080,0x800080,0x008080,0xc0c0c0,0x808080,0xff0000,0x00ff00,0xffff00,0x0000ff,0xff00ff,0x00ffff,0xffffff}; *color=b[idx]; }
        else if (idx<232) { int c=idx-16,r=c/36,g=(c/6)%6,bl=c%6; *color=((r?r*40+55:0)<<16)|((g?g*40+55:0)<<8)|(bl?bl*40+55:0); }
        else { int gray=(idx-232)*10+8; *color=(gray<<16)|(gray<<8)|gray; }
        if (consumed) *consumed=3;
        return 0;
    }
    if (nparams>=5 && params[1]==2) {
        *color=((params[2]&0xFF)<<16)|((params[3]&0xFF)<<8)|(params[4]&0xFF);
        if (consumed) *consumed=5;
        return 0;
    }
    return -1;
}
int ansi256_index_to_rgb(int idx, uint32_t *color) {
    int params[3]={38,5,idx}; int fg, consumed;
    return ansi256_parse(params,3,color,&fg,&consumed);
}
