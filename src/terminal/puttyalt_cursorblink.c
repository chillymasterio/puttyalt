/* puttyalt_cursorblink.c - Cursor blink timing + visibility state. */
#include <stdint.h>
typedef struct { int blink_on; int blink_enabled; uint64_t last_toggle; int interval_ms; int visible; } CursorBlink;
void cursorblink_init(CursorBlink *c, int interval_ms) {
    if(!c) return;
    c->blink_on=1; c->blink_enabled=1; c->last_toggle=0; c->interval_ms=interval_ms>0?interval_ms:500; c->visible=1;
}
int cursorblink_tick(CursorBlink *c, uint64_t now_ms) {
    if(!c||!c->blink_enabled||!c->visible) return c?c->blink_on:0;
    if (now_ms-c->last_toggle>=(uint64_t)c->interval_ms) { c->blink_on=!c->blink_on; c->last_toggle=now_ms; return 1; }
    return 0;
}
void cursorblink_reset(CursorBlink *c, uint64_t now_ms) { if(c){ c->blink_on=1; c->last_toggle=now_ms; } }
void cursorblink_set_enabled(CursorBlink *c, int on) { if(c){ c->blink_enabled=on?1:0; if(!on) c->blink_on=1; } }
void cursorblink_set_visible(CursorBlink *c, int v) { if(c){ c->visible=v?1:0; } }
int cursorblink_is_shown(const CursorBlink *c) { return (c&&c->visible&&(!c->blink_enabled||c->blink_on))?1:0; }
