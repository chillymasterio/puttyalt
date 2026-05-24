#include "puttyalt_smartresize.h"
#include <string.h>

void smartresize_init(SmartResize *sr)
{
    memset(sr, 0, sizeof(*sr));
    sr->enabled = 1;
    sr->min_font_size = 8;
    sr->max_font_size = 24;
    sr->current_font_size = 11;
    sr->target_cols = 80;
    sr->target_rows = 24;
    sr->snap_to_char_grid = 1;
    sr->debounce_ms = 100;
}

int smartresize_calc_font(SmartResize *sr, int win_w, int win_h, int char_w, int char_h)
{
    if (!sr->enabled || char_w <= 0 || char_h <= 0) return sr->current_font_size;
    /* calculate what font size would fit target_cols x target_rows */
    int fw = win_w / sr->target_cols;
    int fh = win_h / sr->target_rows;
    /* use the smaller dimension to ensure it fits */
    int ratio_w = (fw * sr->current_font_size) / char_w;
    int ratio_h = (fh * sr->current_font_size) / char_h;
    int new_size = ratio_w < ratio_h ? ratio_w : ratio_h;
    if (new_size < sr->min_font_size) new_size = sr->min_font_size;
    if (new_size > sr->max_font_size) new_size = sr->max_font_size;
    sr->current_font_size = new_size;
    return new_size;
}

void smartresize_set_target(SmartResize *sr, int cols, int rows)
{
    if (cols > 0) sr->target_cols = cols;
    if (rows > 0) sr->target_rows = rows;
}

int smartresize_snap_window(SmartResize *sr, int *win_w, int *win_h, int char_w, int char_h)
{
    if (!sr->snap_to_char_grid || char_w <= 0 || char_h <= 0) return 0;
    int cols = *win_w / char_w;
    int rows = *win_h / char_h;
    *win_w = cols * char_w;
    *win_h = rows * char_h;
    return 1;
}
