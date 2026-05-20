#include "puttyalt_sidebarresize.h"

void sidebar_resize_init(SidebarResize *sr, int initial_width)
{
    sr->dragging = 0;
    sr->start_x = 0;
    sr->start_width = initial_width;
    sr->current_width = initial_width;
    sr->hover = 0;
    sr->min_width = SIDEBAR_MIN_WIDTH;
    sr->max_width = SIDEBAR_MAX_WIDTH;
    sr->snap_threshold = 60;
    sr->collapsed_width = 0;
}

int sidebar_resize_hit_test(const SidebarResize *sr, int x, int sidebar_right)
{
    (void)sr;
    int zone_left = sidebar_right - SIDEBAR_DRAG_ZONE;
    int zone_right = sidebar_right + SIDEBAR_DRAG_ZONE;
    return x >= zone_left && x <= zone_right;
}

void sidebar_resize_begin(SidebarResize *sr, int x)
{
    sr->dragging = 1;
    sr->start_x = x;
    sr->start_width = sr->current_width;
}

int sidebar_resize_move(SidebarResize *sr, int x)
{
    if (!sr->dragging) return sr->current_width;
    int delta = x - sr->start_x;
    int new_width = sr->start_width + delta;

    /* Snap to collapsed if dragged very narrow */
    if (new_width < sr->snap_threshold) {
        sr->current_width = sr->collapsed_width;
        return sr->current_width;
    }

    if (new_width < sr->min_width) new_width = sr->min_width;
    if (new_width > sr->max_width) new_width = sr->max_width;
    sr->current_width = new_width;
    return new_width;
}

void sidebar_resize_end(SidebarResize *sr)
{
    sr->dragging = 0;
}

int sidebar_resize_get_width(const SidebarResize *sr)
{
    return sr->current_width;
}

int sidebar_resize_is_dragging(const SidebarResize *sr)
{
    return sr->dragging;
}
