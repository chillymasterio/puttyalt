/* puttyalt_panel.c - Dockable panel layout (left/right/top/bottom/float). */
#include <string.h>
#include <stdio.h>
#define PN_MAX 12
#define PN_NAME 32
enum pn_dock { PN_LEFT = 0, PN_RIGHT = 1, PN_TOP = 2, PN_BOTTOM = 3, PN_FLOAT = 4 };
typedef struct {
    char name[PN_NAME];
    int dock;
    int size;
    int visible;
    int x, y, w, h;
} pn_panel;
typedef struct {
    pn_panel panels[PN_MAX];
    int n;
    int container_w, container_h;
} Panel;
void panel_init(Panel *p, int w, int h) {
    if (!p) return;
    memset(p, 0, sizeof(*p));
    p->container_w = w;
    p->container_h = h;
}
int panel_add(Panel *p, const char *name, int dock, int size) {
    if (!p || p->n >= PN_MAX || !name) return -1;
    pn_panel *pn = &p->panels[p->n];
    snprintf(pn->name, PN_NAME, "%s", name);
    pn->dock = dock;
    pn->size = size;
    pn->visible = 1;
    return p->n++;
}
int panel_layout(Panel *p) {
    if (!p) return -1;
    int left = 0, right = p->container_w, top = 0, bottom = p->container_h;
    for (int i = 0; i < p->n; i++) {
        pn_panel *pn = &p->panels[i];
        if (!pn->visible || pn->dock == PN_FLOAT) continue;
        switch (pn->dock) {
            case PN_LEFT:   pn->x = left; pn->y = top; pn->w = pn->size; pn->h = bottom - top; left += pn->size; break;
            case PN_RIGHT:  pn->x = right - pn->size; pn->y = top; pn->w = pn->size; pn->h = bottom - top; right -= pn->size; break;
            case PN_TOP:    pn->x = left; pn->y = top; pn->w = right - left; pn->h = pn->size; top += pn->size; break;
            case PN_BOTTOM: pn->x = left; pn->y = bottom - pn->size; pn->w = right - left; pn->h = pn->size; bottom -= pn->size; break;
        }
    }
    return 0;
}
void panel_toggle(Panel *p, int idx) {
    if (p && idx >= 0 && idx < p->n) p->panels[idx].visible ^= 1;
}
int panel_count(const Panel *p) {
    return p ? p->n : -1;
}
