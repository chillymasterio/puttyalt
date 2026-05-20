#include "puttyalt_cursor.h"
#include "puttyalt_design.h"

void cursor_init(CursorState *cs, CursorStyle style)
{
    cs->style = style;
    cs->visible = 1;
    cs->blink = 1;
    cs->blink_rate_ms = 530;
    cs->thickness = 2;
    cs->color = DS_TERM_CURSOR;
    cs->color_inverted = DS_TERM_BG;
    cs->opacity = 1.0f;
    cs->last_blink = 0;
    cs->blink_state = 1;
}

void cursor_set_style(CursorState *cs, CursorStyle style)
{
    cs->style = style;
    if (style == CURSOR_BAR) cs->thickness = 2;
    else if (style == CURSOR_UNDERLINE) cs->thickness = 2;
    else cs->thickness = 0;
}

void cursor_set_blink(CursorState *cs, int enable, int rate_ms)
{
    cs->blink = enable;
    if (rate_ms > 0) cs->blink_rate_ms = rate_ms;
    if (!enable) cs->blink_state = 1;
}

int cursor_update(CursorState *cs, long now_ms)
{
    if (!cs->blink) { cs->blink_state = 1; return 0; }
    if (now_ms - cs->last_blink >= cs->blink_rate_ms) {
        cs->blink_state = !cs->blink_state;
        cs->last_blink = now_ms;
        return 1;  /* changed */
    }
    return 0;
}

int cursor_is_visible(const CursorState *cs)
{
    return cs->visible && cs->blink_state;
}

void cursor_get_rect(const CursorState *cs, int x, int y,
                     int char_w, int char_h,
                     int *rx, int *ry, int *rw, int *rh)
{
    switch (cs->style) {
    case CURSOR_BLOCK:
        *rx = x; *ry = y; *rw = char_w; *rh = char_h;
        break;
    case CURSOR_UNDERLINE:
        *rx = x; *ry = y + char_h - cs->thickness;
        *rw = char_w; *rh = cs->thickness;
        break;
    case CURSOR_BAR:
        *rx = x; *ry = y; *rw = cs->thickness; *rh = char_h;
        break;
    case CURSOR_HOLLOW:
        *rx = x; *ry = y; *rw = char_w; *rh = char_h;
        break;
    }
}
