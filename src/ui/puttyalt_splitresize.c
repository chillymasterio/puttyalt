/* puttyalt_splitresize.c - Split pane interactive resize with constraints. */
#include <string.h>
typedef struct { int divider_pos; int min_a, min_b; int total; int dragging; int start_pos; } SplitResize;
void splitresize_init(SplitResize *s, int total, int min_a, int min_b) {
    if(!s) return; memset(s,0,sizeof(*s)); s->total=total; s->min_a=min_a; s->min_b=min_b; s->divider_pos=total/2;
}
int splitresize_begin(SplitResize *s, int mouse_pos) {
    if(!s) return -1; s->dragging=1; s->start_pos=mouse_pos; return 0;
}
int splitresize_drag(SplitResize *s, int mouse_pos) {
    if(!s||!s->dragging) return -1;
    int delta=mouse_pos-s->start_pos;
    int newpos=s->divider_pos+delta;
    if (newpos<s->min_a) newpos=s->min_a;
    if (newpos>s->total-s->min_b) newpos=s->total-s->min_b;
    s->divider_pos=newpos; s->start_pos=mouse_pos;
    return newpos;
}
void splitresize_end(SplitResize *s) { if(s) s->dragging=0; }
int splitresize_pane_a(const SplitResize *s) { return s?s->divider_pos:-1; }
int splitresize_pane_b(const SplitResize *s) { return s?(s->total-s->divider_pos):-1; }
float splitresize_ratio(const SplitResize *s) { return (s&&s->total)?(float)s->divider_pos/s->total:0; }
