#ifndef PUTTYALT_SIDEBARRESIZE_H
#define PUTTYALT_SIDEBARRESIZE_H

#define SIDEBAR_MIN_WIDTH    140
#define SIDEBAR_MAX_WIDTH    500
#define SIDEBAR_DRAG_ZONE    5

typedef struct {
    int dragging;
    int start_x;
    int start_width;
    int current_width;
    int hover;
    int min_width;
    int max_width;
    int snap_threshold;
    int collapsed_width;
} SidebarResize;

void sidebar_resize_init(SidebarResize *sr, int initial_width);
int  sidebar_resize_hit_test(const SidebarResize *sr, int x, int sidebar_right);
void sidebar_resize_begin(SidebarResize *sr, int x);
int  sidebar_resize_move(SidebarResize *sr, int x);
void sidebar_resize_end(SidebarResize *sr);
int  sidebar_resize_get_width(const SidebarResize *sr);
int  sidebar_resize_is_dragging(const SidebarResize *sr);

#endif
