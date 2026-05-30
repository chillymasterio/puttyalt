/* puttyalt_panelsnap.c - floating panel snap-to-edge tracking for PuttyAlt */
#include <string.h>
#include <stdio.h>

#define PANELSNAP_MAX_PANELS 32

typedef struct {
    int id;       /* panel identifier, >= 0 when in use */
    int x, y;     /* top-left position within container */
    int w, h;     /* panel dimensions */
    int used;     /* slot occupancy flag */
} PanelRect;

typedef struct {
    PanelRect panels[PANELSNAP_MAX_PANELS];
    int count;
    int container_w;
    int container_h;
    int threshold;   /* magnetic snap distance in pixels */
} PanelSnapState;

static int ps_iabs(int v) { return v < 0 ? -v : v; }

static int ps_find(const PanelSnapState *s, int panel_id) {
    int i;
    for (i = 0; i < PANELSNAP_MAX_PANELS; i++)
        if (s->panels[i].used && s->panels[i].id == panel_id)
            return i;
    return -1;
}

void panelsnap_init(PanelSnapState *s, int container_w, int container_h, int threshold) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
    s->container_w = container_w < 0 ? 0 : container_w;
    s->container_h = container_h < 0 ? 0 : container_h;
    s->threshold = threshold < 0 ? 0 : threshold;
}

int panelsnap_register(PanelSnapState *s, int panel_id, int x, int y, int w, int h) {
    int i;
    if (!s || panel_id < 0 || w <= 0 || h <= 0) return -1;
    if (ps_find(s, panel_id) >= 0) return -1;
    for (i = 0; i < PANELSNAP_MAX_PANELS; i++) {
        if (!s->panels[i].used) {
            s->panels[i].used = 1;
            s->panels[i].id = panel_id;
            s->panels[i].x = x;
            s->panels[i].y = y;
            s->panels[i].w = w;
            s->panels[i].h = h;
            s->count++;
            return 0;
        }
    }
    return -1;
}

int panelsnap_drag(PanelSnapState *s, int panel_id, int new_x, int new_y, int *out_x, int *out_y) {
    int idx, i, sx, sy, t;
    PanelRect *p;
    if (!s) return -1;
    idx = ps_find(s, panel_id);
    if (idx < 0) return -1;
    p = &s->panels[idx];
    sx = new_x;
    sy = new_y;
    t = s->threshold;
    /* snap to container edges */
    if (ps_iabs(new_x) <= t) sx = 0;
    if (ps_iabs(new_y) <= t) sy = 0;
    if (ps_iabs((new_x + p->w) - s->container_w) <= t) sx = s->container_w - p->w;
    if (ps_iabs((new_y + p->h) - s->container_h) <= t) sy = s->container_h - p->h;
    /* snap to sibling panel edges */
    for (i = 0; i < PANELSNAP_MAX_PANELS; i++) {
        PanelRect *o = &s->panels[i];
        if (!o->used || o->id == panel_id) continue;
        if (ps_iabs(new_x - (o->x + o->w)) <= t) sx = o->x + o->w;
        if (ps_iabs((new_x + p->w) - o->x) <= t) sx = o->x - p->w;
        if (ps_iabs(new_y - (o->y + o->h)) <= t) sy = o->y + o->h;
        if (ps_iabs((new_y + p->h) - o->y) <= t) sy = o->y - p->h;
    }
    p->x = sx;
    p->y = sy;
    if (out_x) *out_x = sx;
    if (out_y) *out_y = sy;
    return 0;
}

int panelsnap_resize_container(PanelSnapState *s, int container_w, int container_h) {
    int i, clamped = 0;
    if (!s || container_w < 0 || container_h < 0) return -1;
    s->container_w = container_w;
    s->container_h = container_h;
    for (i = 0; i < PANELSNAP_MAX_PANELS; i++) {
        PanelRect *p = &s->panels[i];
        if (!p->used) continue;
        if (p->x + p->w > container_w) { p->x = container_w - p->w; clamped++; }
        if (p->y + p->h > container_h) { p->y = container_h - p->h; clamped++; }
        if (p->x < 0) { p->x = 0; clamped++; }
        if (p->y < 0) { p->y = 0; clamped++; }
    }
    return clamped;
}

int panelsnap_nearest_edge(const PanelSnapState *s, int panel_id, char *out_edge, int cap) {
    int idx, dl, dr, dt, db, best;
    const PanelRect *p;
    const char *name;
    if (!s || !out_edge || cap <= 0) return -1;
    idx = ps_find(s, panel_id);
    if (idx < 0) return -1;
    p = &s->panels[idx];
    dl = ps_iabs(p->x);
    dr = ps_iabs(s->container_w - (p->x + p->w));
    dt = ps_iabs(p->y);
    db = ps_iabs(s->container_h - (p->y + p->h));
    best = dl; name = "left";
    if (dr < best) { best = dr; name = "right"; }
    if (dt < best) { best = dt; name = "top"; }
    if (db < best) { best = db; name = "bottom"; }
    if (snprintf(out_edge, (size_t)cap, "%s", name) < 0) return -1;
    return best;
}

void panelsnap_release(PanelSnapState *s, int panel_id) {
    int idx;
    if (!s) return;
    idx = ps_find(s, panel_id);
    if (idx < 0) return;
    s->panels[idx].used = 0;
    s->panels[idx].id = 0;
    if (s->count > 0) s->count--;
}
