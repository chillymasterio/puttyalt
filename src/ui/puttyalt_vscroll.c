/* puttyalt_vscroll.c - Virtual scroll viewport for huge lists (windowed rendering). */
#include <string.h>
#include <stdio.h>
typedef struct { int total_items; int item_h; int viewport_h; int scroll_y; int overscan; } VScroll;
void vscroll_init(VScroll *v, int item_h, int viewport_h) {
    if(!v) return;
    memset(v,0,sizeof(*v));
    v->item_h = item_h>0?item_h:18; v->viewport_h=viewport_h>0?viewport_h:0; v->overscan=2;
}
void vscroll_set_total(VScroll *v, int total) { if(v) v->total_items = total<0?0:total; }
int vscroll_max_scroll(const VScroll *v) {
    if(!v) return 0;
    int content=v->total_items*v->item_h;
    int m=content - v->viewport_h; return m>0?m:0;
}
void vscroll_scroll_to(VScroll *v, int y) {
    if(!v) return;
    int m=vscroll_max_scroll(v);
    v->scroll_y = y<0?0:(y>m?m:y);
}
void vscroll_scroll_by(VScroll *v, int dy) { if(v) vscroll_scroll_to(v, v->scroll_y+dy); }
int vscroll_first_visible(const VScroll *v) {
    if(!v||v->item_h<=0) return 0;
    int i=v->scroll_y/v->item_h - v->overscan; return i<0?0:i;
}
int vscroll_visible_count(const VScroll *v) {
    if(!v||v->item_h<=0) return 0;
    int n=v->viewport_h/v->item_h + 2*v->overscan + 1;
    return n>v->total_items?v->total_items:n;
}
int vscroll_item_y(const VScroll *v, int index) {
    if(!v) return 0;
    return index*v->item_h - v->scroll_y;
}
