/* puttyalt_bwgraph.c - Bandwidth graph ring buffer with up/down series. */
#include <string.h>
#include <stdint.h>
#define BG_SAMPLES 120
typedef struct { uint32_t up[BG_SAMPLES]; uint32_t down[BG_SAMPLES]; int head, count; uint32_t peak_up, peak_down; } BwGraph;
void bwgraph_init(BwGraph *b) { if(b) memset(b,0,sizeof(*b)); }
void bwgraph_sample(BwGraph *b, uint32_t up_bps, uint32_t down_bps) {
    if(!b) return;
    b->up[b->head]=up_bps; b->down[b->head]=down_bps;
    b->head=(b->head+1)%BG_SAMPLES; if (b->count<BG_SAMPLES) b->count++;
    if (up_bps>b->peak_up) b->peak_up=up_bps;
    if (down_bps>b->peak_down) b->peak_down=down_bps;
}
uint32_t bwgraph_avg_up(const BwGraph *b) {
    if(!b||b->count==0) return 0;
    uint64_t s=0; for(int i=0;i<b->count;i++) s+=b->up[i]; return (uint32_t)(s/b->count);
}
uint32_t bwgraph_avg_down(const BwGraph *b) {
    if(!b||b->count==0) return 0;
    uint64_t s=0; for(int i=0;i<b->count;i++) s+=b->down[i]; return (uint32_t)(s/b->count);
}
int bwgraph_normalized(const BwGraph *b, int down, int *out, int cap) {
    if(!b) return -1;
    uint32_t peak = down?b->peak_down:b->peak_up; if(peak==0) peak=1;
    int n=b->count<cap?b->count:cap;
    for (int i=0;i<n;i++) {
        int idx=(b->head - b->count + i + BG_SAMPLES)%BG_SAMPLES;
        uint32_t v = down?b->down[idx]:b->up[idx];
        out[i]=(int)((v*100)/peak);
    }
    return n;
}
int bwgraph_count(const BwGraph *b) { return b?b->count:-1; }
