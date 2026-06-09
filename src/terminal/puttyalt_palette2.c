/* puttyalt_palette2.c - Dynamic palette with OSC 4 color set/query. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
typedef struct { uint32_t colors[256]; int modified[256]; } Palette2;
void palette2_init(Palette2 *p) {
    if(!p) return; memset(p,0,sizeof(*p));
    /* init xterm 256 palette compactly */
    static const uint32_t basic[16]={0x000000,0x800000,0x008000,0x808000,0x000080,0x800080,0x008080,0xc0c0c0,
        0x808080,0xff0000,0x00ff00,0xffff00,0x0000ff,0xff00ff,0x00ffff,0xffffff};
    for (int i=0;i<16;i++) p->colors[i]=basic[i];
    for (int i=16;i<232;i++) { int c=i-16,r=c/36,g=(c/6)%6,b=c%6;
        p->colors[i]=((r?r*40+55:0)<<16)|((g?g*40+55:0)<<8)|(b?b*40+55:0); }
    for (int i=232;i<256;i++) { int gray=(i-232)*10+8; p->colors[i]=(gray<<16)|(gray<<8)|gray; }
}
int palette2_set(Palette2 *p, int idx, uint32_t color) {
    if(!p||idx<0||idx>255) return -1; p->colors[idx]=color; p->modified[idx]=1; return 0;
}
uint32_t palette2_get(const Palette2 *p, int idx) {
    return (p&&idx>=0&&idx<256)?p->colors[idx]:0;
}
int palette2_query(const Palette2 *p, int idx, char *out, int outlen) {
    if(!p||idx<0||idx>255) return -1;
    uint32_t c=p->colors[idx]; int r=(c>>16)&0xFF,g=(c>>8)&0xFF,b=c&0xFF;
    return snprintf(out,outlen,"\x1b]4;%d;rgb:%02x%02x/%02x%02x/%02x%02x\x07",idx,r,r,g,g,b,b);
}
int palette2_reset(Palette2 *p, int idx) {
    if(!p) return -1;
    if (idx<0) { palette2_init(p); return 0; }
    if (idx>255) return -1;
    Palette2 fresh; palette2_init(&fresh); p->colors[idx]=fresh.colors[idx]; p->modified[idx]=0;
    return 0;
}
int palette2_modified_count(const Palette2 *p) {
    if(!p) return -1; int n=0; for(int i=0;i<256;i++) if(p->modified[i])n++; return n;
}
