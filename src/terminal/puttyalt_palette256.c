/* puttyalt_palette256.c - xterm 256-color palette to RGB resolver. */
#include <stdint.h>
uint32_t palette256_rgb(int idx) {
    if (idx<0||idx>255) return 0;
    static const int basic[16]={0x000000,0x800000,0x008000,0x808000,0x000080,0x800080,0x008080,0xc0c0c0,
        0x808080,0xff0000,0x00ff00,0xffff00,0x0000ff,0xff00ff,0x00ffff,0xffffff};
    if (idx<16) return (uint32_t)basic[idx];
    if (idx<232) {
        int c=idx-16, r=c/36, g=(c/6)%6, b=c%6;
        int rv=r?r*40+55:0, gv=g?g*40+55:0, bv=b?b*40+55:0;
        return (uint32_t)((rv<<16)|(gv<<8)|bv);
    }
    int gray=(idx-232)*10+8;
    return (uint32_t)((gray<<16)|(gray<<8)|gray);
}
int palette256_nearest(uint32_t rgb) {
    int best=0, bestd=1<<30;
    for (int i=0;i<256;i++) {
        uint32_t c=palette256_rgb(i);
        int dr=((c>>16)&0xFF)-((rgb>>16)&0xFF), dg=((c>>8)&0xFF)-((rgb>>8)&0xFF), db=(c&0xFF)-(rgb&0xFF);
        int d=dr*dr+dg*dg+db*db;
        if (d<bestd) { bestd=d; best=i; }
    }
    return best;
}
