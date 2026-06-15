/* puttyalt_palette256.c - Map 256-color index to RGB.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
void pal_to_rgb(int idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    if (idx < 0) idx = 0;
    if (idx > 255) idx = 255;
    if (idx < 16) {
        static const uint8_t basic[16][3] = {
            {0,0,0},{128,0,0},{0,128,0},{128,128,0},{0,0,128},{128,0,128},{0,128,128},{192,192,192},
            {128,128,128},{255,0,0},{0,255,0},{255,255,0},{0,0,255},{255,0,255},{0,255,255},{255,255,255}
        };
        *r=basic[idx][0]; *g=basic[idx][1]; *b=basic[idx][2];
    } else if (idx < 232) {
        int c = idx - 16;
        int ri = c / 36, gi = (c % 36) / 6, bi = c % 6;
        static const uint8_t lvl[6] = {0,95,135,175,215,255};
        *r=lvl[ri]; *g=lvl[gi]; *b=lvl[bi];
    } else {
        uint8_t v = (uint8_t)(8 + (idx - 232) * 10);
        *r=*g=*b=v;
    }
}
int pal_nearest(uint8_t r, uint8_t g, uint8_t b) {
    int best=0; long bd=1L<<30;
    for (int i=0;i<256;i++) {
        uint8_t pr,pg,pb; pal_to_rgb(i,&pr,&pg,&pb);
        long d=(long)(pr-r)*(pr-r)+(long)(pg-g)*(pg-g)+(long)(pb-b)*(pb-b);
        if (d<bd){bd=d;best=i;}
    }
    return best;
}
