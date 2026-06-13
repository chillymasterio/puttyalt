/* puttyalt_scrollregion.c - DECSTBM scroll region (top/bottom margins) handling. */
#include <string.h>
typedef struct { int top, bottom; int rows; int origin_mode; } ScrollRegion;
void scrollregion_init(ScrollRegion *s, int rows) {
    if(!s) return;
    s->rows=rows; s->top=0; s->bottom=rows-1; s->origin_mode=0;
}
int scrollregion_set(ScrollRegion *s, int top, int bottom) {
    if(!s) return -1;
    if (top<0) top=0;
    if (bottom>=s->rows) bottom=s->rows-1;
    if (top>=bottom) return -1;
    s->top=top; s->bottom=bottom; return 0;
}
void scrollregion_reset(ScrollRegion *s) { if(s){ s->top=0; s->bottom=s->rows-1; } }
int scrollregion_contains(const ScrollRegion *s, int row) {
    if(!s) return 0;
    return (row>=s->top && row<=s->bottom)?1:0;
}
int scrollregion_clamp_cursor(const ScrollRegion *s, int row) {
    if(!s) return row;
    if (s->origin_mode) { if (row<s->top) return s->top; if (row>s->bottom) return s->bottom; }
    return row;
}
void scrollregion_set_origin_mode(ScrollRegion *s, int on) { if(s) s->origin_mode=on?1:0; }
int scrollregion_height(const ScrollRegion *s) { return s? (s->bottom-s->top+1):0; }
