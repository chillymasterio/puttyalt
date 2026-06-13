/* puttyalt_wrapstate.c - Line wrap state tracking (pending wrap, DECAWM). */
#include <string.h>
typedef struct { int autowrap; int pending_wrap; int cursor_col; int cols; int reverse_wrap; } WrapState;
void wrapstate_init(WrapState *w, int cols) {
    if(!w) return;
    memset(w,0,sizeof(*w)); w->autowrap=1; w->cols=cols>0?cols:80;
}
void wrapstate_set_autowrap(WrapState *w, int on) { if(w) w->autowrap=on?1:0; }
/* Returns 1 if a wrap (newline) should occur before printing. */
int wrapstate_advance(WrapState *w) {
    if(!w) return 0;
    if (w->pending_wrap && w->autowrap) {
        w->pending_wrap=0; w->cursor_col=0; return 1; /* wrap to next line */
    }
    w->cursor_col++;
    if (w->cursor_col >= w->cols) {
        if (w->autowrap) w->pending_wrap=1;
        /* defer wrap (VT100 behavior) */
        else w->cursor_col=w->cols-1; /* clamp at margin */
    }
    return 0;
}
void wrapstate_set_col(WrapState *w, int col) { if(w){ w->cursor_col=col; w->pending_wrap=0; } }
void wrapstate_resize(WrapState *w, int cols) { if(w){ w->cols=cols; if(w->cursor_col>=cols) w->cursor_col=cols-1; w->pending_wrap=0; } }
int wrapstate_at_margin(const WrapState *w) { return w?w->pending_wrap:-1; }
