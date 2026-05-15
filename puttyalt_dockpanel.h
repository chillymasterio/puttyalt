#ifndef PUTTYALT_DOCKPANEL_H
#define PUTTYALT_DOCKPANEL_H

#define DOCK_MAX_PANELS 16

typedef enum { DOCK_LEFT, DOCK_RIGHT, DOCK_TOP, DOCK_BOTTOM, DOCK_CENTER, DOCK_FLOAT } DockPosition;

typedef struct {
    int id;
    char title[64];
    DockPosition position;
    int x, y, width, height;
    int min_width, min_height;
    int visible;
    int collapsed;
    float size_ratio;
} DockPanel;

typedef struct {
    DockPanel panels[DOCK_MAX_PANELS];
    int panel_count;
    int container_w, container_h;
    int splitter_size;
    int dragging_splitter;
    int drag_panel_id;
} DockLayout;

void dock_init(DockLayout *dl);
int  dock_add_panel(DockLayout *dl, int id, const char *title, DockPosition pos, float ratio);
void dock_remove_panel(DockLayout *dl, int id);
void dock_toggle_panel(DockLayout *dl, int id);
void dock_layout(DockLayout *dl, int container_w, int container_h);
DockPanel *dock_get_panel(DockLayout *dl, int id);
DockPanel *dock_panel_at(DockLayout *dl, int x, int y);

#endif
