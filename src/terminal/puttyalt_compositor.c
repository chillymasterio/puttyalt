/* puttyalt_compositor.c - Layer compositor: z-ordered surfaces with damage merge. */
#include <string.h>
#include <stdio.h>
#define CO_MAX_LAYERS 16
typedef struct { int id; int z; int x, y, w, h; int opacity; int visible; int dirty; } co_layer;
typedef struct { co_layer l[CO_MAX_LAYERS]; int n; int dmg_x0, dmg_y0, dmg_x1, dmg_y1; int has_damage; } Compositor;
void compositor_init(Compositor *c) { if(c) memset(c,0,sizeof(*c)); }
int compositor_add_layer(Compositor *c, int id, int z, int x, int y, int w, int h) {
    if(!c||c->n>=CO_MAX_LAYERS) return -1;
    co_layer *l=&c->l[c->n++]; l->id=id; l->z=z; l->x=x; l->y=y; l->w=w; l->h=h;
    l->opacity=255; l->visible=1; l->dirty=1; return 0;
}
static void co_mark(Compositor *c, int x, int y, int w, int h) {
    if (!c->has_damage) { c->dmg_x0=x; c->dmg_y0=y; c->dmg_x1=x+w; c->dmg_y1=y+h; c->has_damage=1; }
    else {
        if (x<c->dmg_x0) c->dmg_x0=x;
        if (y<c->dmg_y0) c->dmg_y0=y;
        if (x+w>c->dmg_x1) c->dmg_x1=x+w;
        if (y+h>c->dmg_y1) c->dmg_y1=y+h;
    }
}
int compositor_move_layer(Compositor *c, int id, int nx, int ny) {
    if(!c) return -1;
    for (int i=0;i<c->n;i++) if (c->l[i].id==id) {
        co_mark(c,c->l[i].x,c->l[i].y,c->l[i].w,c->l[i].h);
        c->l[i].x=nx; c->l[i].y=ny; c->l[i].dirty=1;
        co_mark(c,nx,ny,c->l[i].w,c->l[i].h); return 0;
    }
    return -1;
}
int compositor_set_opacity(Compositor *c, int id, int opacity) {
    if(!c) return -1;
    for (int i=0;i<c->n;i++) if (c->l[i].id==id) { c->l[i].opacity=opacity<0?0:(opacity>255?255:opacity); c->l[i].dirty=1; co_mark(c,c->l[i].x,c->l[i].y,c->l[i].w,c->l[i].h); return 0; }
    return -1;
}
int compositor_damage_rect(const Compositor *c, int *x, int *y, int *w, int *h) {
    if(!c||!c->has_damage) return 0;
    if(x) *x=c->dmg_x0;
    if(y)*y=c->dmg_y0;
    if(w)*w=c->dmg_x1-c->dmg_x0;
    if(h)*h=c->dmg_y1-c->dmg_y0;
    return 1;
}
void compositor_clear_damage(Compositor *c) { if(c){ c->has_damage=0; for(int i=0;i<c->n;i++) c->l[i].dirty=0; } }
int compositor_topmost_at(const Compositor *c, int px, int py) {
    if(!c) return -1;
    int best=-1, bestz=-1;
    for (int i=0;i<c->n;i++) {
        co_layer *l=&c->l[i];
        if (l->visible && px>=l->x && px<l->x+l->w && py>=l->y && py<l->y+l->h && l->z>bestz) { bestz=l->z; best=l->id; }
    }
    return best;
}
