/* puttyalt_colorscheme2.c - Color scheme v2 with 16-color + bright palette. */
#include <string.h>
#include <stdint.h>
typedef struct { uint32_t palette[16]; uint32_t fg; uint32_t bg; uint32_t cursor; uint32_t selection; char name[32]; } ColorScheme2;
void colorscheme2_init(ColorScheme2 *c, const char *name) {
    if(!c) return;
    memset(c,0,sizeof(*c));
    if(name){ int i=0; for(;name[i]&&i<31;i++) c->name[i]=name[i]; c->name[i]=0; }
    /* default dark palette */
    uint32_t def[16]={0x000000,0xCC0000,0x4E9A06,0xC4A000,0x3465A4,0x75507B,0x06989A,0xD3D7CF,
                      0x555753,0xEF2929,0x8AE234,0xFCE94F,0x729FCF,0xAD7FA8,0x34E2E2,0xEEEEEC};
    for (int i=0;i<16;i++) c->palette[i]=def[i];
    c->fg=0xD3D7CF; c->bg=0x000000; c->cursor=0xFFFFFF; c->selection=0x444444;
}
void colorscheme2_set_color(ColorScheme2 *c, int idx, uint32_t color) {
    if(c&&idx>=0&&idx<16) c->palette[idx]=color;
}
uint32_t colorscheme2_resolve(const ColorScheme2 *c, int idx, int bold) {
    if(!c) return 0;
    if (idx<0||idx>15) return c->fg;
    if (bold && idx<8) return c->palette[idx+8];
    /* bold -> bright variant */
    return c->palette[idx];
}
uint32_t colorscheme2_contrast_fg(const ColorScheme2 *c, uint32_t bg) {
    if(!c) return 0xFFFFFF;
    int lum=((bg>>16&0xFF)*30+(bg>>8&0xFF)*59+(bg&0xFF)*11)/100;
    return lum>128 ? 0x000000 : 0xFFFFFF;
}
