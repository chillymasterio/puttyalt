/* puttyalt_chanwin.c - SSH channel flow-control window accounting. */
#include <stdint.h>
typedef struct { int32_t local_win, remote_win; int32_t initial; int32_t adjust_threshold; uint64_t consumed; } ChanWin;
void chanwin_init(ChanWin *w, int32_t initial) {
    if(!w) return;
    w->local_win=w->remote_win=w->initial=initial>0?initial:131072;
    w->adjust_threshold=w->initial/2; w->consumed=0;
}
int chanwin_consume_local(ChanWin *w, int32_t n) {
    if(!w||n<0) return -1;
    if (n>w->local_win) return -1;
    w->local_win-=n; w->consumed+=n; return 0;
}
int chanwin_needs_adjust(const ChanWin *w) { return (w && w->local_win<=w->adjust_threshold)?1:0; }
int32_t chanwin_adjust_local(ChanWin *w) {
    if(!w) return 0;
    int32_t add=w->initial - w->local_win; w->local_win=w->initial; return add;
}
int chanwin_consume_remote(ChanWin *w, int32_t n) {
    if(!w||n<0||n>w->remote_win) return -1;
    w->remote_win-=n; return 0;
}
void chanwin_grow_remote(ChanWin *w, int32_t n) { if(w&&n>0) w->remote_win+=n; }
int32_t chanwin_remote_avail(const ChanWin *w) { return w?w->remote_win:0; }
