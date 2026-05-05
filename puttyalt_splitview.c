/*
 * puttyalt_splitview.c: Terminal split-pane implementation.
 */

#include <string.h>
#include "puttyalt_splitview.h"

void splitview_init(SplitLayout *layout, int cols, int rows)
{
    memset(layout, 0, sizeof(*layout));
    layout->total_cols = cols;
    layout->total_rows = rows;
    layout->num_panes = 1;
    layout->active_pane = 0;

    SplitPane *p = &layout->panes[0];
    p->id = 0;
    p->x = 0;
    p->y = 0;
    p->w = cols;
    p->h = rows;
    p->session_idx = 0;
    p->active = 1;
    p->split_dir = SPLIT_NONE;
}

int splitview_split(SplitLayout *layout, int pane_idx, SplitDirection dir)
{
    if (layout->num_panes >= SPLIT_MAX_PANES)
        return -1;
    if (pane_idx < 0 || pane_idx >= layout->num_panes)
        return -1;

    SplitPane *src = &layout->panes[pane_idx];
    int new_idx = layout->num_panes;
    SplitPane *dst = &layout->panes[new_idx];

    if (dir == SPLIT_HORIZONTAL) {
        int half = src->h / 2;
        if (half < SPLIT_MIN_ROWS)
            return -1;
        dst->x = src->x;
        dst->y = src->y + half;
        dst->w = src->w;
        dst->h = src->h - half;
        src->h = half;
    } else if (dir == SPLIT_VERTICAL) {
        int half = src->w / 2;
        if (half < SPLIT_MIN_COLS)
            return -1;
        dst->x = src->x + half;
        dst->y = src->y;
        dst->w = src->w - half;
        dst->h = src->h;
        src->w = half;
    } else {
        return -1;
    }

    dst->id = new_idx;
    dst->session_idx = -1; /* no session assigned yet */
    dst->active = 0;
    dst->split_dir = dir;
    src->split_dir = dir;

    layout->num_panes++;
    return new_idx;
}

int splitview_close(SplitLayout *layout, int pane_idx)
{
    if (layout->num_panes <= 1)
        return -1;
    if (pane_idx < 0 || pane_idx >= layout->num_panes)
        return -1;

    /* Shift remaining panes down */
    for (int i = pane_idx; i < layout->num_panes - 1; i++)
        layout->panes[i] = layout->panes[i + 1];

    layout->num_panes--;

    if (layout->active_pane >= layout->num_panes)
        layout->active_pane = layout->num_panes - 1;

    /* Redistribute space evenly */
    splitview_resize(layout, layout->total_cols, layout->total_rows);
    return 0;
}

void splitview_resize(SplitLayout *layout, int new_cols, int new_rows)
{
    layout->total_cols = new_cols;
    layout->total_rows = new_rows;

    if (layout->num_panes == 1) {
        layout->panes[0].x = 0;
        layout->panes[0].y = 0;
        layout->panes[0].w = new_cols;
        layout->panes[0].h = new_rows;
        return;
    }

    /* Simple equal distribution for now */
    int per_pane_h = new_rows / layout->num_panes;
    for (int i = 0; i < layout->num_panes; i++) {
        layout->panes[i].x = 0;
        layout->panes[i].y = i * per_pane_h;
        layout->panes[i].w = new_cols;
        layout->panes[i].h = per_pane_h;
    }
    /* Give remainder to last pane */
    layout->panes[layout->num_panes - 1].h =
        new_rows - (layout->num_panes - 1) * per_pane_h;
}

int splitview_focus_next(SplitLayout *layout)
{
    layout->panes[layout->active_pane].active = 0;
    layout->active_pane = (layout->active_pane + 1) % layout->num_panes;
    layout->panes[layout->active_pane].active = 1;
    return layout->active_pane;
}

int splitview_focus_prev(SplitLayout *layout)
{
    layout->panes[layout->active_pane].active = 0;
    layout->active_pane = (layout->active_pane + layout->num_panes - 1)
                          % layout->num_panes;
    layout->panes[layout->active_pane].active = 1;
    return layout->active_pane;
}

void splitview_get_pane_size(const SplitLayout *layout, int pane_idx,
                             int *cols, int *rows)
{
    if (pane_idx < 0 || pane_idx >= layout->num_panes) {
        *cols = 0;
        *rows = 0;
        return;
    }
    *cols = layout->panes[pane_idx].w;
    *rows = layout->panes[pane_idx].h;
}
