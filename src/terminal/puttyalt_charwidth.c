/* puttyalt_charwidth.c - East-Asian wide / combining char width classifier. */
#include <stdint.h>
int charwidth(uint32_t cp) {
    if (cp==0) return 0;
    if (cp<32 || (cp>=0x7f && cp<0xa0)) return -1; /* control */
    /* combining marks */
    if ((cp>=0x300 && cp<=0x36f)||(cp>=0x1ab0&&cp<=0x1aff)||(cp>=0x20d0&&cp<=0x20ff)) return 0;
    /* zero-width */
    if (cp==0x200b||cp==0xfeff) return 0;
    /* wide ranges (CJK, Hangul, fullwidth, emoji approx) */
    if ((cp>=0x1100&&cp<=0x115f)||(cp>=0x2e80&&cp<=0x303e)||(cp>=0x3041&&cp<=0x33ff)||
        (cp>=0x3400&&cp<=0x4dbf)||(cp>=0x4e00&&cp<=0x9fff)||(cp>=0xa000&&cp<=0xa4cf)||
        (cp>=0xac00&&cp<=0xd7a3)||(cp>=0xf900&&cp<=0xfaff)||(cp>=0xff00&&cp<=0xff60)||
        (cp>=0xffe0&&cp<=0xffe6)||(cp>=0x1f300&&cp<=0x1faff)||(cp>=0x20000&&cp<=0x3fffd)) return 2;
    return 1;
}
int charwidth_str_cols(const uint32_t *cps, int n) {
    int cols=0; for (int i=0;i<n;i++) { int w=charwidth(cps[i]); if (w>0) cols+=w; }
    return cols;
}
