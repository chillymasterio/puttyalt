#ifndef PUTTYALT_CURSOR_H
#define PUTTYALT_CURSOR_H

typedef enum {
    CURSOR_BLOCK,
    CURSOR_UNDERLINE,
    CURSOR_BAR,
    CURSOR_HOLLOW
} CursorStyle;

typedef struct {
    CursorStyle style;
    int visible;
    int blink;
    int blink_rate_ms;
    int thickness;
    unsigned int color;
    unsigned int color_inverted;
    float opacity;
    long last_blink;
    int blink_state;
} CursorState;

void cursor_init(CursorState *cs, CursorStyle style);
void cursor_set_style(CursorState *cs, CursorStyle style);
void cursor_set_blink(CursorState *cs, int enable, int rate_ms);
int  cursor_update(CursorState *cs, long now_ms);
int  cursor_is_visible(const CursorState *cs);
void cursor_get_rect(const CursorState *cs, int x, int y,
                     int char_w, int char_h,
                     int *rx, int *ry, int *rw, int *rh);

#endif
