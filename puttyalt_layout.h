#ifndef PUTTYALT_LAYOUT_H
#define PUTTYALT_LAYOUT_H

#define LAYOUT_MAX_PANES    16
#define LAYOUT_MAX_SAVED    32
#define LAYOUT_MAX_NAME     64

typedef enum {
    PANE_TERMINAL = 0,
    PANE_SFTP,
    PANE_HEXVIEW,
    PANE_LOG,
    PANE_EMPTY
} PaneType;

typedef struct {
    PaneType type;
    int      x, y, w, h;     /* percentage-based 0-100 */
    int      session_id;
    int      focused;
} LayoutPane;

typedef struct {
    char       name[LAYOUT_MAX_NAME];
    LayoutPane panes[LAYOUT_MAX_PANES];
    int        pane_count;
    int        sidebar_visible;
    int        sidebar_width;
    int        statusbar_visible;
} SavedLayout;

typedef struct {
    SavedLayout layouts[LAYOUT_MAX_SAVED];
    int         count;
    int         active;
    LayoutPane  current[LAYOUT_MAX_PANES];
    int         current_count;
} LayoutMgr;

void layout_init(LayoutMgr *lm);
int  layout_save(LayoutMgr *lm, const char *name);
int  layout_restore(LayoutMgr *lm, int index);
int  layout_find(const LayoutMgr *lm, const char *name);
int  layout_remove(LayoutMgr *lm, int index);
int  layout_add_pane(LayoutMgr *lm, PaneType type, int x, int y, int w, int h);
int  layout_remove_pane(LayoutMgr *lm, int index);
int  layout_resize_pane(LayoutMgr *lm, int index, int w, int h);
int  layout_load_file(LayoutMgr *lm, const char *path);
int  layout_save_file(const LayoutMgr *lm, const char *path);

#endif
