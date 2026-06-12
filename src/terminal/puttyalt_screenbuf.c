/* puttyalt_screenbuf.c - Screen buffer dirty-line tracking for partial redraw. */
#include <string.h>
#define SB_MAXROWS 256
typedef struct { unsigned char dirty[SB_MAXROWS]; int rows; int all_dirty; } ScreenBuf;
void screenbuf_init(ScreenBuf *s, int rows) {
    if(!s) return;
    memset(s,0,sizeof(*s)); s->rows=rows>SB_MAXROWS?SB_MAXROWS:rows; s->all_dirty=1;
}
void screenbuf_mark_dirty(ScreenBuf *s, int row) { if(s&&row>=0&&row<s->rows) s->dirty[row]=1; }
void screenbuf_mark_range(ScreenBuf *s, int from, int to) {
    if(!s) return;
    for(int i=from;i<=to&&i<s->rows;i++) if(i>=0) s->dirty[i]=1;
}
void screenbuf_mark_all(ScreenBuf *s) { if(s) s->all_dirty=1; }
int screenbuf_is_dirty(const ScreenBuf *s, int row) {
    if(!s) return 1;
    if(s->all_dirty) return 1; return (row>=0&&row<s->rows)?s->dirty[row]:0;
}
int screenbuf_dirty_count(const ScreenBuf *s) {
    if(!s) return -1;
    if(s->all_dirty) return s->rows;
    int n=0; for(int i=0;i<s->rows;i++) if(s->dirty[i])n++; return n;
}
void screenbuf_clear_dirty(ScreenBuf *s) {
    if(!s) return;
    memset(s->dirty,0,sizeof(s->dirty)); s->all_dirty=0;
}
void screenbuf_scroll(ScreenBuf *s, int lines) {
    if(!s) return;
    s->all_dirty=1; (void)lines;
}
