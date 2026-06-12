/* puttyalt_hicontrast.c - High-contrast accessibility theme adapter (WCAG ratios). */
#include <string.h>
#include <stdio.h>
typedef struct { unsigned int fg, bg, accent; int min_ratio_x10; int active; } HiContrast;
static double hc_lum(unsigned int c) {
    double r=((c>>16)&0xFF)/255.0, g=((c>>8)&0xFF)/255.0, b=(c&0xFF)/255.0;
    r = r<=0.03928? r/12.92 : 0; /* simplified */
    return 0.2126*((c>>16&0xFF)/255.0)+0.7152*((c>>8&0xFF)/255.0)+0.0722*((c&0xFF)/255.0);
}
void hicontrast_init(HiContrast *h, int min_ratio_x10) {
    if(!h) return;
    memset(h,0,sizeof(*h));
    h->min_ratio_x10 = min_ratio_x10>0?min_ratio_x10:45; /* 4.5:1 default */
    h->fg=0xFFFFFF; h->bg=0x000000; h->accent=0xFFFF00;
}
int hicontrast_ratio_x10(unsigned int fg, unsigned int bg) {
    double l1=hc_lum(fg), l2=hc_lum(bg);
    if (l1<l2) { double t=l1; l1=l2; l2=t; }
    double ratio=(l1+0.05)/(l2+0.05);
    return (int)(ratio*10+0.5);
}
int hicontrast_passes(const HiContrast *h, unsigned int fg, unsigned int bg) {
    if(!h) return -1;
    return hicontrast_ratio_x10(fg,bg) >= h->min_ratio_x10 ? 1 : 0;
}
unsigned int hicontrast_adjust_fg(const HiContrast *h, unsigned int fg, unsigned int bg) {
    if(!h) return fg;
    if (hicontrast_passes(h,fg,bg)) return fg;
    /* flip to white or black, whichever passes */
    return hicontrast_ratio_x10(0xFFFFFF,bg) >= hicontrast_ratio_x10(0x000000,bg) ? 0xFFFFFF : 0x000000;
}
void hicontrast_set_active(HiContrast *h, int on) { if(h) h->active=on?1:0; }
int hicontrast_is_active(const HiContrast *h) { return h?h->active:-1; }
