/* puttyalt_colorquant.c - Truecolor to 256/16-palette quantizer with caching. */
#include <stdint.h>
#define CQ_CACHE 64
typedef struct { uint32_t rgb; int idx; } cq_cache_entry;
typedef struct { cq_cache_entry cache[CQ_CACHE]; int n; int hits, misses; } ColorQuant;
void colorquant_init(ColorQuant *c) { if(c){ for(int i=0;i<CQ_CACHE;i++){c->cache[i].rgb=0;c->cache[i].idx=-1;} c->n=0; c->hits=0; c->misses=0; } }
static int cq_to_256(uint32_t rgb) {
    int r=(rgb>>16)&0xFF, g=(rgb>>8)&0xFF, b=rgb&0xFF;
    if (r==g && g==b) { if (r<8) return 16; if (r>248) return 231; return 232 + (r-8)/10; }
    int ri=r*5/255, gi=g*5/255, bi=b*5/255;
    return 16 + 36*ri + 6*gi + bi;
}
int colorquant_to_256(ColorQuant *c, uint32_t rgb) {
    if(!c) return cq_to_256(rgb);
    for (int i=0;i<c->n;i++) if (c->cache[i].rgb==rgb) { c->hits++; return c->cache[i].idx; }
    c->misses++; int idx=cq_to_256(rgb);
    if (c->n<CQ_CACHE) { c->cache[c->n].rgb=rgb; c->cache[c->n].idx=idx; c->n++; }
    return idx;
}
int colorquant_to_16(uint32_t rgb) {
    int r=(rgb>>16)&0xFF, g=(rgb>>8)&0xFF, b=rgb&0xFF;
    int bright = (r>128||g>128||b>128)?8:0;
    return bright | ((r>96?1:0) | (g>96?2:0) | (b>96?4:0));
}
int colorquant_hit_rate(const ColorQuant *c) { if(!c) return -1; int t=c->hits+c->misses; return t?(c->hits*100)/t:0; }
