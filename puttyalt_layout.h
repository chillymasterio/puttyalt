#ifndef PUTTYALT_LAYOUT_H
#define PUTTYALT_LAYOUT_H

#define LAYOUT_MAX_PRESETS  16
#define LAYOUT_NAME_LEN     64
#define LAYOUT_MAX_PANES    8

typedef struct LayoutPane {
    int x_pct, y_pct;       /* position as percentage */
    int w_pct, h_pct;       /* size as percentage */
    int session_template;   /* template index to use */
} LayoutPane;

typedef struct LayoutPreset {
    char name[LAYOUT_NAME_LEN];
    LayoutPane panes[LAYOUT_MAX_PANES];
    int num_panes;
} LayoutPreset;

typedef struct LayoutManager {
    LayoutPreset presets[LAYOUT_MAX_PRESETS];
    int count;
} LayoutManager;

void layout_init(LayoutManager *lm);
int  layout_create(LayoutManager *lm, const char *name);
int  layout_add_pane(LayoutManager *lm, int preset_idx,
                     int x_pct, int y_pct, int w_pct, int h_pct);
int  layout_delete(LayoutManager *lm, int index);
int  layout_apply(const LayoutManager *lm, int index);
int  layout_save(const LayoutManager *lm, const char *path);
int  layout_load(LayoutManager *lm, const char *path);

#endif
