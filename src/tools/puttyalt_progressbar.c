/* puttyalt_progressbar.c - Progress bar renderer with ETA + rate display. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
typedef struct { uint64_t total, current; uint64_t start_ms; int width; } ProgressBar;
void progressbar_init(ProgressBar *p, uint64_t total, int width, uint64_t now_ms) {
    if(!p) return; p->total=total; p->current=0; p->start_ms=now_ms; p->width=width>0?width:30;
}
void progressbar_update(ProgressBar *p, uint64_t current) { if(p) p->current=current; }
int progressbar_pct(const ProgressBar *p) {
    if(!p||p->total==0) return 0; return (int)((p->current*100)/p->total);
}
int progressbar_render(const ProgressBar *p, uint64_t now_ms, char *buf, int buflen) {
    if(!p||!buf) return -1;
    int pct=progressbar_pct(p);
    int filled=(pct*p->width)/100;
    int pos=0; buf[pos++]='[';
    for (int i=0;i<p->width && pos<buflen-1;i++) buf[pos++] = i<filled?'#':'-';
    if (pos<buflen-1) buf[pos++]=']';
    /* ETA */
    uint64_t elapsed=now_ms-p->start_ms;
    if (p->current>0 && elapsed>0) {
        uint64_t rate=(p->current*1000)/elapsed; /* bytes/sec */
        uint64_t remain = p->total>p->current ? (p->total-p->current) : 0;
        int eta = rate>0 ? (int)(remain/rate) : 0;
        pos+=snprintf(buf+pos,buflen-pos," %d%% ETA %ds",pct,eta);
    } else pos+=snprintf(buf+pos,buflen-pos," %d%%",pct);
    return pos;
}
int progressbar_done(const ProgressBar *p) { return (p&&p->current>=p->total&&p->total>0)?1:0; }
