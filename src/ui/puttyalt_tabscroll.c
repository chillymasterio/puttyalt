/* puttyalt_tabscroll.c - Tab bar horizontal scroll + overflow chevron logic. */
#include <string.h>
typedef struct { int tab_widths[64]; int n; int scroll_x; int viewport_w; int active_tab; } TabScroll;
void tabscroll_init(TabScroll *t, int viewport_w) { if(t){ memset(t,0,sizeof(*t)); t->viewport_w=viewport_w; } }
int tabscroll_add(TabScroll *t, int width) {
    if(!t||t->n>=64) return -1; t->tab_widths[t->n]=width; return t->n++;
}
int tabscroll_total_width(const TabScroll *t) {
    if(!t) return 0; int w=0; for(int i=0;i<t->n;i++) w+=t->tab_widths[i]; return w;
}
int tabscroll_overflows(const TabScroll *t) { return t? (tabscroll_total_width(t)>t->viewport_w):0; }
int tabscroll_tab_x(const TabScroll *t, int idx) {
    if(!t||idx<0||idx>=t->n) return -1; int x=0; for(int i=0;i<idx;i++) x+=t->tab_widths[i]; return x - t->scroll_x;
}
void tabscroll_ensure_visible(TabScroll *t, int idx) {
    if(!t||idx<0||idx>=t->n) return;
    int x=0; for(int i=0;i<idx;i++) x+=t->tab_widths[i];
    int right=x+t->tab_widths[idx];
    if (x<t->scroll_x) t->scroll_x=x;
    else if (right>t->scroll_x+t->viewport_w) t->scroll_x=right-t->viewport_w;
    if (t->scroll_x<0) t->scroll_x=0;
}
int tabscroll_can_scroll_left(const TabScroll *t) { return t? (t->scroll_x>0):0; }
int tabscroll_can_scroll_right(const TabScroll *t) { return t? (t->scroll_x+t->viewport_w < tabscroll_total_width(t)):0; }
