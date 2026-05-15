#include "puttyalt_dockpanel.h"
#include <string.h>
#include <stdio.h>

void dock_init(DockLayout *dl)
{
    memset(dl, 0, sizeof(*dl));
    dl->splitter_size = 4;
}

int dock_add_panel(DockLayout *dl, int id, const char *title, DockPosition pos, float ratio)
{
    if (dl->panel_count >= DOCK_MAX_PANELS) return -1;
    DockPanel *p = &dl->panels[dl->panel_count++];
    memset(p, 0, sizeof(*p));
    p->id = id;
    snprintf(p->title, sizeof(p->title), "%s", title);
    p->position = pos;
    p->size_ratio = ratio > 0.0f ? ratio : 0.25f;
    p->visible = 1;
    p->min_width = 100;
    p->min_height = 60;
    return 0;
}

void dock_remove_panel(DockLayout *dl, int id)
{
    for (int i = 0; i < dl->panel_count; i++) {
        if (dl->panels[i].id == id) {
            memmove(&dl->panels[i], &dl->panels[i + 1], (dl->panel_count - i - 1) * sizeof(DockPanel));
            dl->panel_count--;
            return;
        }
    }
}

void dock_toggle_panel(DockLayout *dl, int id)
{
    DockPanel *p = dock_get_panel(dl, id);
    if (p) p->visible = !p->visible;
}

void dock_layout(DockLayout *dl, int cw, int ch)
{
    dl->container_w = cw;
    dl->container_h = ch;
    int left = 0, right = cw, top = 0, bottom = ch;

    for (int i = 0; i < dl->panel_count; i++) {
        DockPanel *p = &dl->panels[i];
        if (!p->visible) continue;
        switch (p->position) {
        case DOCK_LEFT:
            p->width = (int)(cw * p->size_ratio);
            if (p->width < p->min_width) p->width = p->min_width;
            p->x = left; p->y = top; p->height = bottom - top;
            left += p->width + dl->splitter_size;
            break;
        case DOCK_RIGHT:
            p->width = (int)(cw * p->size_ratio);
            if (p->width < p->min_width) p->width = p->min_width;
            p->x = right - p->width; p->y = top; p->height = bottom - top;
            right -= p->width + dl->splitter_size;
            break;
        case DOCK_TOP:
            p->height = (int)(ch * p->size_ratio);
            if (p->height < p->min_height) p->height = p->min_height;
            p->x = left; p->y = top; p->width = right - left;
            top += p->height + dl->splitter_size;
            break;
        case DOCK_BOTTOM:
            p->height = (int)(ch * p->size_ratio);
            if (p->height < p->min_height) p->height = p->min_height;
            p->x = left; p->y = bottom - p->height; p->width = right - left;
            bottom -= p->height + dl->splitter_size;
            break;
        case DOCK_CENTER:
            p->x = left; p->y = top; p->width = right - left; p->height = bottom - top;
            break;
        case DOCK_FLOAT:
            break;
        }
    }
}

DockPanel *dock_get_panel(DockLayout *dl, int id)
{
    for (int i = 0; i < dl->panel_count; i++)
        if (dl->panels[i].id == id) return &dl->panels[i];
    return NULL;
}

DockPanel *dock_panel_at(DockLayout *dl, int x, int y)
{
    for (int i = dl->panel_count - 1; i >= 0; i--) {
        DockPanel *p = &dl->panels[i];
        if (p->visible && x >= p->x && x < p->x + p->width && y >= p->y && y < p->y + p->height) return p;
    }
    return NULL;
}
