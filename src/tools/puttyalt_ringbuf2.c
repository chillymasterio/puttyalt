/* puttyalt_ringbuf2.c - Generic byte ring buffer with overflow tracking. */
#include <string.h>
#define RB_SIZE 8192
typedef struct { unsigned char data[RB_SIZE]; int head, tail, count; unsigned long overflow; } RingBuf2;
void ringbuf2_init(RingBuf2 *r) { if(r) memset(r,0,sizeof(*r)); }
int ringbuf2_write(RingBuf2 *r, const unsigned char *src, int len) {
    if(!r||!src) return -1; int written=0;
    for (int i=0;i<len;i++) {
        if (r->count>=RB_SIZE) { r->overflow++; r->tail=(r->tail+1)%RB_SIZE; r->count--; }
        r->data[r->head]=src[i]; r->head=(r->head+1)%RB_SIZE; r->count++; written++;
    }
    return written;
}
int ringbuf2_read(RingBuf2 *r, unsigned char *dst, int max) {
    if(!r||!dst) return -1; int n=0;
    while (r->count>0 && n<max) { dst[n++]=r->data[r->tail]; r->tail=(r->tail+1)%RB_SIZE; r->count--; }
    return n;
}
int ringbuf2_available(const RingBuf2 *r) { return r?r->count:-1; }
int ringbuf2_free(const RingBuf2 *r) { return r?(RB_SIZE-r->count):-1; }
unsigned long ringbuf2_overflow_count(const RingBuf2 *r) { return r?r->overflow:0; }
void ringbuf2_clear(RingBuf2 *r) { if(r){ r->head=r->tail=r->count=0; } }
