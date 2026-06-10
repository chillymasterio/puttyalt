/* puttyalt_radialmenu.c - Radial/pie context menu geometry. */
#include <string.h>
#include <stdio.h>
#define RM_MAX 12
#define RM_LABEL 24
typedef struct { char label[RM_LABEL]; int action_id; } rm_item;
typedef struct { rm_item items[RM_MAX]; int n; int center_x, center_y; int radius; int hovered; } RadialMenu;
void radialmenu_init(RadialMenu *r, int cx, int cy, int radius) {
    if(!r) return;
    memset(r,0,sizeof(*r)); r->center_x=cx; r->center_y=cy; r->radius=radius; r->hovered=-1;
}
int radialmenu_add(RadialMenu *r, const char *label, int action_id) {
    if(!r||r->n>=RM_MAX||!label) return -1;
    snprintf(r->items[r->n].label,RM_LABEL,"%s",label); r->items[r->n].action_id=action_id; return r->n++;
}
/* simple integer angle approximation (no math.h) using octants */
static int rm_octant(int dx, int dy) {
    int adx=dx<0?-dx:dx, ady=dy<0?-dy:dy;
    int oct;
    if (dx>=0 && dy<0) oct = adx>ady?0:1;       /* upper right */
    else if (dx<0 && dy<0) oct = ady>adx?2:3;   /* upper left */
    else if (dx<0 && dy>=0) oct = adx<ady?4:5;  /* lower left */
    else oct = ady>adx?6:7;                      /* lower right */
    return oct;
}
int radialmenu_hit(RadialMenu *r, int px, int py) {
    if(!r||r->n==0) return -1;
    int dx=px-r->center_x, dy=py-r->center_y;
    int dist2=dx*dx+dy*dy;
    if (dist2 > r->radius*r->radius || dist2 < (r->radius/4)*(r->radius/4)) { r->hovered=-1; return -1; }
    int sector = rm_octant(dx,dy) * r->n / 8;
    if (sector>=r->n) sector=r->n-1;
    r->hovered=sector; return sector;
}
int radialmenu_hovered(const RadialMenu *r) { return r?r->hovered:-1; }
int radialmenu_count(const RadialMenu *r) { return r?r->n:-1; }
