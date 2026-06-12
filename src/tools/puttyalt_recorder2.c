/* puttyalt_recorder2.c - Session recorder v2: timed frame log with seek index. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define RC2_MAX 256
#define RC2_DATA 96
typedef struct { uint64_t t_ms; char data[RC2_DATA]; int len; } rc2_frame;
typedef struct { rc2_frame f[RC2_MAX]; int n; uint64_t start_ms; int recording; } Recorder2;
void recorder2_init(Recorder2 *r) { if(r) memset(r,0,sizeof(*r)); }
int recorder2_start(Recorder2 *r, uint64_t now_ms) {
    if(!r) return -1;
    r->n=0; r->start_ms=now_ms; r->recording=1; return 0;
}
int recorder2_capture(Recorder2 *r, const char *data, int len, uint64_t now_ms) {
    if(!r||!r->recording||!data) return -1;
    if (r->n>=RC2_MAX) return -1;
    rc2_frame *f=&r->f[r->n++]; f->t_ms=now_ms-r->start_ms;
    int cl=len<RC2_DATA-1?len:RC2_DATA-1; memcpy(f->data,data,cl); f->data[cl]=0; f->len=cl;
    return 0;
}
void recorder2_stop(Recorder2 *r) { if(r) r->recording=0; }
int recorder2_seek(const Recorder2 *r, uint64_t t_ms) {
    if(!r||r->n==0) return -1;
    int lo=0, hi=r->n-1, res=0;
    while (lo<=hi) { int mid=(lo+hi)/2; if (r->f[mid].t_ms<=t_ms) { res=mid; lo=mid+1; } else hi=mid-1; }
    return res;
}
uint64_t recorder2_duration(const Recorder2 *r) { return (r&&r->n>0)?r->f[r->n-1].t_ms:0; }
int recorder2_frame_count(const Recorder2 *r) { return r?r->n:-1; }
