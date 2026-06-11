/* puttyalt_mempool2.c - Fixed-block memory pool allocator. */
#include <string.h>
#define MP_BLOCKS 256
typedef struct { unsigned char used[MP_BLOCKS]; int block_size; int total_blocks; int allocated; int peak; } MemPool2;
void mempool2_init(MemPool2 *m, int block_size, int total_blocks) {
    if(!m) return;
    memset(m,0,sizeof(*m)); m->block_size=block_size;
    m->total_blocks=total_blocks<MP_BLOCKS?total_blocks:MP_BLOCKS;
}
int mempool2_alloc(MemPool2 *m) {
    if(!m) return -1;
    for (int i=0;i<m->total_blocks;i++) if (!m->used[i]) {
        m->used[i]=1; m->allocated++; if(m->allocated>m->peak) m->peak=m->allocated; return i;
    }
    return -1; /* pool exhausted */
}
int mempool2_free(MemPool2 *m, int block) {
    if(!m||block<0||block>=m->total_blocks||!m->used[block]) return -1;
    m->used[block]=0; m->allocated--; return 0;
}
int mempool2_available(const MemPool2 *m) { return m?(m->total_blocks-m->allocated):-1; }
int mempool2_peak(const MemPool2 *m) { return m?m->peak:-1; }
int mempool2_offset(const MemPool2 *m, int block) { return (m&&block>=0)?block*m->block_size:-1; }
