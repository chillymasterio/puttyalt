/* puttyalt_sixel2.c - Sixel graphics v2: palette registers + raster attributes. */
#include <string.h>
#include <stdint.h>
#define SX_COLORS 256
typedef struct { uint32_t palette[SX_COLORS]; int ncolors; int width, height; int aspect_num, aspect_den; int cur_color; } Sixel2;
void sixel2_init(Sixel2 *s) { if(s){ memset(s,0,sizeof(*s)); s->aspect_num=1; s->aspect_den=1; } }
int sixel2_set_raster(Sixel2 *s, int pan, int pad, int w, int h) {
    if(!s||pad<=0) return -1; s->aspect_num=pan; s->aspect_den=pad; s->width=w; s->height=h; return 0;
}
int sixel2_define_color(Sixel2 *s, int reg, int r, int g, int b) {
    if(!s||reg<0||reg>=SX_COLORS) return -1;
    /* sixel colors are 0-100 percent */
    int rr=r*255/100, gg=g*255/100, bb=b*255/100;
    s->palette[reg]=((rr&0xFF)<<16)|((gg&0xFF)<<8)|(bb&0xFF);
    if (reg>=s->ncolors) s->ncolors=reg+1;
    return 0;
}
int sixel2_select_color(Sixel2 *s, int reg) {
    if(!s||reg<0||reg>=SX_COLORS) return -1; s->cur_color=reg; return 0;
}
uint32_t sixel2_color(const Sixel2 *s, int reg) {
    return (s&&reg>=0&&reg<SX_COLORS)?s->palette[reg]:0;
}
int sixel2_decode_sixel(unsigned char ch) {
    if (ch<0x3f || ch>0x7e) return -1;
    return ch - 0x3f; /* 6-bit vertical bitmask */
}
int sixel2_color_count(const Sixel2 *s) { return s?s->ncolors:-1; }
