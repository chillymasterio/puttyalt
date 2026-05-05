/*
 * puttyalt_splitview.h: Terminal split-pane support.
 *
 * Allows splitting the terminal area horizontally or vertically
 * to view multiple sessions side by side.
 */

#ifndef PUTTYALT_SPLITVIEW_H
#define PUTTYALT_SPLITVIEW_H

#define SPLIT_MAX_PANES    4
#define SPLIT_MIN_COLS     20
#define SPLIT_MIN_ROWS     5

typedef enum {
    SPLIT_NONE = 0,
    SPLIT_HORIZONTAL,
    SPLIT_VERTICAL
} SplitDirection;

typedef struct SplitPane {
    int id;
    int x, y, w, h;          /* position in character cells */
    int session_idx;          /* index into tab manager */
    int active;               /* is this the focused pane? */
    SplitDirection split_dir; /* how this pane was split */
} SplitPane;

typedef struct SplitLayout {
    SplitPane panes[SPLIT_MAX_PANES];
    int num_panes;
    int active_pane;
    int total_cols;
    int total_rows;
} SplitLayout;

void splitview_init(SplitLayout *layout, int cols, int rows);
int  splitview_split(SplitLayout *layout, int pane_idx, SplitDirection dir);
int  splitview_close(SplitLayout *layout, int pane_idx);
void splitview_resize(SplitLayout *layout, int new_cols, int new_rows);
int  splitview_focus_next(SplitLayout *layout);
int  splitview_focus_prev(SplitLayout *layout);
void splitview_get_pane_size(const SplitLayout *layout, int pane_idx,
                             int *cols, int *rows);

#endif /* PUTTYALT_SPLITVIEW_H */
