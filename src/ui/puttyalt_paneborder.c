/* puttyalt_paneborder.c - Split pane border rendering + active highlight. */
#include <string.h>
typedef struct { int active_pane; int border_width; unsigned int active_color; unsigned int inactive_color; int show_titles; } PaneBorder;
void paneborder_init(PaneBorder *p) {
    if(!p) return; memset(p,0,sizeof(*p)); p->active_pane=-1; p->border_width=1;
    p->active_color=0x6C5CE7; p->inactive_color=0x2A2A3E; p->show_titles=1;
}
void paneborder_set_active(PaneBorder *p, int pane) { if(p) p->active_pane=pane; }
unsigned int paneborder_color(const PaneBorder *p, int pane) {
    if(!p) return 0; return (pane==p->active_pane)?p->active_color:p->inactive_color;
}
int paneborder_width(const PaneBorder *p, int pane) {
    if(!p) return 1; return (pane==p->active_pane)?p->border_width+1:p->border_width;
}
void paneborder_set_colors(PaneBorder *p, unsigned int active, unsigned int inactive) {
    if(!p) return; p->active_color=active; p->inactive_color=inactive;
}
int paneborder_is_active(const PaneBorder *p, int pane) { return (p&&pane==p->active_pane)?1:0; }
