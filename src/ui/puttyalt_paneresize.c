/* puttyalt_paneresize.c - Pane divider resize logic for PuttyAlt.
 * Adjusts a divider's split ratio during a drag, clamping each child to a
 * minimum cell size and snapping ratios to the cell grid. Pure logic. */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define PANERESIZE_MAX_LABEL 64

typedef struct PaneResizeState {
    int   active;        /* 1 while a drag is in progress, else 0 */
    int   divider_id;    /* id of the divider being dragged, -1 if none */
    int   start_pos;     /* mouse position (px along axis) at drag begin */
    int   min_cols;      /* minimum columns for either child pane */
    int   min_rows;      /* minimum rows for either child pane */
    float last_ratio;    /* most recently computed split ratio [0,1] */
    char  label[PANERESIZE_MAX_LABEL]; /* human-readable status string */
} PaneResizeState;

void paneresize_init(PaneResizeState *r, int min_cols, int min_rows)
{
    if (!r)
        return;
    memset(r, 0, sizeof(*r));
    r->active = 0;
    r->divider_id = -1;
    r->start_pos = 0;
    r->min_cols = (min_cols > 0) ? min_cols : 1;
    r->min_rows = (min_rows > 0) ? min_rows : 1;
    r->last_ratio = 0.5f;
    snprintf(r->label, sizeof(r->label), "idle");
}

int paneresize_begin(PaneResizeState *r, int divider_id, int mouse_pos)
{
    if (!r || divider_id < 0)
        return -1;
    if (r->active)
        return -1; /* a drag is already in progress */
    r->active = 1;
    r->divider_id = divider_id;
    r->start_pos = mouse_pos;
    snprintf(r->label, sizeof(r->label), "drag div=%d", divider_id);
    return 0;
}

float paneresize_clamp_ratio(const PaneResizeState *r, float ratio,
                             int axis_extent, int cell_px)
{
    int m, min_px, snapped, cells, max_cells;
    float lo, hi, snapped_ratio;

    if (!r || axis_extent <= 0 || cell_px <= 0)
        return 0.5f;

    /* Larger of the two minima keeps both child panes safe in either axis. */
    m = (r->min_cols > r->min_rows) ? r->min_cols : r->min_rows;
    min_px = m * cell_px;
    if (2 * min_px >= axis_extent)
        return 0.5f; /* not enough room; centre the divider */

    /* Snap the pixel boundary to a whole cell, then clamp to the band so
     * neither child shrinks below its minimum cell size. */
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    max_cells = axis_extent / cell_px;
    cells = (int)(ratio * (float)axis_extent / (float)cell_px + 0.5f);
    if (cells < 0) cells = 0;
    if (cells > max_cells) cells = max_cells;
    snapped = cells * cell_px;

    lo = (float)min_px / (float)axis_extent;
    hi = (float)(axis_extent - min_px) / (float)axis_extent;
    snapped_ratio = (float)snapped / (float)axis_extent;
    if (snapped_ratio < lo) snapped_ratio = lo;
    if (snapped_ratio > hi) snapped_ratio = hi;
    return snapped_ratio;
}

float paneresize_drag(PaneResizeState *r, int mouse_pos, int axis_extent)
{
    float raw;

    if (!r || !r->active || axis_extent <= 0)
        return (r ? r->last_ratio : 0.5f);

    raw = (float)mouse_pos / (float)axis_extent;
    if (raw < 0.0f) raw = 0.0f;
    if (raw > 1.0f) raw = 1.0f;

    r->last_ratio = raw;
    snprintf(r->label, sizeof(r->label), "drag div=%d r=%.3f",
             r->divider_id, (double)raw);
    return raw;
}

void paneresize_end(PaneResizeState *r)
{
    if (!r)
        return;
    r->active = 0;
    r->divider_id = -1;
    r->start_pos = 0;
    snprintf(r->label, sizeof(r->label), "idle");
}
