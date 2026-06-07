/* puttyalt_imgrender.c - Inline image render placement + scroll tracking. */
#include <string.h>
#define IR_MAX 16
typedef struct { int img_id; int top_row; int height_rows; int width_cols; int col; int visible; } ir_placement;
typedef struct { ir_placement p[IR_MAX]; int n; int scroll_offset; } ImgRender;
void imgrender_init(ImgRender *r) { if(r) memset(r,0,sizeof(*r)); }
int imgrender_place(ImgRender *r, int img_id, int row, int col, int width_cols, int height_rows) {
    if(!r||r->n>=IR_MAX) return -1;
    ir_placement *p=&r->p[r->n++]; p->img_id=img_id; p->top_row=row; p->col=col;
    p->width_cols=width_cols; p->height_rows=height_rows; p->visible=1; return 0;
}
void imgrender_scroll(ImgRender *r, int lines) {
    if(!r) return; r->scroll_offset+=lines;
    for (int i=0;i<r->n;i++) {
        int screen_row = r->p[i].top_row - r->scroll_offset;
        r->p[i].visible = (screen_row + r->p[i].height_rows > 0);
    }
}
int imgrender_at_row(const ImgRender *r, int screen_row) {
    if(!r) return -1;
    for (int i=0;i<r->n;i++) if (r->p[i].visible) {
        int top=r->p[i].top_row - r->scroll_offset;
        if (screen_row>=top && screen_row<top+r->p[i].height_rows) return r->p[i].img_id;
    }
    return -1;
}
int imgrender_visible_count(const ImgRender *r) {
    if(!r) return -1; int n=0; for(int i=0;i<r->n;i++) if(r->p[i].visible)n++; return n;
}
