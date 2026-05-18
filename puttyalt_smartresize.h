#ifndef PUTTYALT_SMARTRESIZE_H
#define PUTTYALT_SMARTRESIZE_H

typedef struct {
    int enabled;
    int min_font_size;
    int max_font_size;
    int current_font_size;
    int target_cols;
    int target_rows;
    int preserve_aspect;
    int snap_to_char_grid;
    int debounce_ms;
} SmartResize;

void smartresize_init(SmartResize *sr);
int  smartresize_calc_font(SmartResize *sr, int win_w, int win_h, int char_w, int char_h);
void smartresize_set_target(SmartResize *sr, int cols, int rows);
int  smartresize_snap_window(SmartResize *sr, int *win_w, int *win_h, int char_w, int char_h);

#endif
