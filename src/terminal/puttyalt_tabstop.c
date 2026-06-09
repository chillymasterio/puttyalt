/* puttyalt_tabstop.c - Terminal tab stop manager (HTS/TBC). */
#include <string.h>
#define TS_MAX 256
typedef struct { unsigned char stops[TS_MAX]; int cols; } TabStop;
void tabstop_init(TabStop *t, int cols) {
    if(!t) return; memset(t,0,sizeof(*t)); t->cols=cols>TS_MAX?TS_MAX:cols;
    for (int i=0;i<t->cols;i+=8) t->stops[i]=1; /* default every 8 */
}
void tabstop_set(TabStop *t, int col) { if(t&&col>=0&&col<t->cols) t->stops[col]=1; }
void tabstop_clear(TabStop *t, int col) { if(t&&col>=0&&col<t->cols) t->stops[col]=0; }
void tabstop_clear_all(TabStop *t) { if(t) for(int i=0;i<t->cols;i++) t->stops[i]=0; }
int tabstop_next(const TabStop *t, int from_col) {
    if(!t) return from_col+1;
    for (int i=from_col+1;i<t->cols;i++) if (t->stops[i]) return i;
    return t->cols-1;
}
int tabstop_prev(const TabStop *t, int from_col) {
    if(!t) return from_col-1<0?0:from_col-1;
    for (int i=from_col-1;i>=0;i--) if (t->stops[i]) return i;
    return 0;
}
int tabstop_count(const TabStop *t) {
    if(!t) return -1; int n=0; for(int i=0;i<t->cols;i++) if(t->stops[i])n++; return n;
}
