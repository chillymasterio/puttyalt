/* puttyalt_rsync.c - Rsync-style rolling checksum + block matching (delta sync). */
#include <string.h>
#include <stdint.h>
#define RS_BLOCK 1024
typedef struct { uint32_t weak; uint32_t strong; int block_idx; } rs_sig;
typedef struct { rs_sig sigs[256]; int n; int block_size; } Rsync;
void rsync_init(Rsync *r, int block_size) {
    if(!r) return;
    memset(r,0,sizeof(*r)); r->block_size=block_size>0?block_size:RS_BLOCK;
}
uint32_t rsync_weak_checksum(const unsigned char *data, int len) {
    uint32_t a=0, b=0;
    for (int i=0;i<len;i++) { a+=data[i]; b+=(len-i)*data[i]; }
    return (a & 0xFFFF) | (b << 16);
}
uint32_t rsync_strong_checksum(const unsigned char *data, int len) {
    uint32_t h=2166136261u;
    for (int i=0;i<len;i++) { h^=data[i]; h*=16777619u; }
    return h;
}
int rsync_add_block_sig(Rsync *r, const unsigned char *block, int len, int block_idx) {
    if(!r||r->n>=256) return -1;
    rs_sig *s=&r->sigs[r->n++]; s->weak=rsync_weak_checksum(block,len); s->strong=rsync_strong_checksum(block,len); s->block_idx=block_idx;
    return 0;
}
int rsync_match(const Rsync *r, const unsigned char *block, int len) {
    if(!r) return -1;
    uint32_t w=rsync_weak_checksum(block,len);
    for (int i=0;i<r->n;i++) if (r->sigs[i].weak==w) {
        if (r->sigs[i].strong==rsync_strong_checksum(block,len)) return r->sigs[i].block_idx;
    }
    return -1;
}
int rsync_sig_count(const Rsync *r) { return r?r->n:-1; }
