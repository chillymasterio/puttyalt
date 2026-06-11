/* puttyalt_listview.c - List view with selection, scroll, filter. */
#include <string.h>
#include <stdio.h>
#define LV_MAX 256
#define LV_TEXT 96
typedef struct { char text[LV_TEXT]; int id; int selected; int visible; } lv_item;
typedef struct { lv_item items[LV_MAX]; int n; int cursor; int scroll; int viewport_rows; } ListView;
void listview_init(ListView *l, int viewport_rows) {
    if(!l) return; memset(l,0,sizeof(*l)); l->viewport_rows=viewport_rows>0?viewport_rows:10; l->cursor=-1;
}
int listview_add(ListView *l, const char *text, int id) {
    if(!l||l->n>=LV_MAX) return -1;
    snprintf(l->items[l->n].text,LV_TEXT,"%s",text?text:""); l->items[l->n].id=id; l->items[l->n].visible=1;
    if (l->cursor<0) l->cursor=0; return l->n++;
}
void listview_filter(ListView *l, const char *query) {
    if(!l) return;
    for (int i=0;i<l->n;i++) l->items[i].visible = (!query||!*query||strstr(l->items[i].text,query))?1:0;
}
int listview_move(ListView *l, int delta) {
    if(!l||l->n==0) return -1;
    int c=l->cursor;
    do { c+=delta>0?1:-1; if(c<0)c=0; if(c>=l->n)c=l->n-1; } while (c>0 && c<l->n-1 && !l->items[c].visible);
    l->cursor=c;
    if (c<l->scroll) l->scroll=c;
    if (c>=l->scroll+l->viewport_rows) l->scroll=c-l->viewport_rows+1;
    return c;
}
void listview_toggle(ListView *l) { if(l&&l->cursor>=0&&l->cursor<l->n) l->items[l->cursor].selected^=1; }
int listview_selected_id(const ListView *l) { return (l&&l->cursor>=0&&l->cursor<l->n)?l->items[l->cursor].id:-1; }
int listview_visible_count(const ListView *l) { if(!l) return -1; int n=0; for(int i=0;i<l->n;i++) if(l->items[i].visible)n++; return n; }
