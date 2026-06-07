/* puttyalt_tooltip2.c - Tooltip v2 with delay, positioning, rich content. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TT_TEXT 256
typedef struct { char text[TT_TEXT]; int x, y; int show_delay_ms; int visible; uint64_t hover_start; int target_id; } Tooltip2;
void tooltip2_init(Tooltip2 *t, int delay_ms) {
    if(!t) return; memset(t,0,sizeof(*t)); t->show_delay_ms=delay_ms>0?delay_ms:500; t->target_id=-1;
}
void tooltip2_hover(Tooltip2 *t, int target_id, const char *text, int x, int y, uint64_t now_ms) {
    if(!t) return;
    if (t->target_id!=target_id) { t->target_id=target_id; t->hover_start=now_ms; t->visible=0;
        snprintf(t->text,TT_TEXT,"%s",text?text:""); t->x=x; t->y=y; }
}
void tooltip2_leave(Tooltip2 *t) { if(t){ t->target_id=-1; t->visible=0; } }
int tooltip2_tick(Tooltip2 *t, uint64_t now_ms) {
    if(!t||t->target_id<0) return 0;
    if (!t->visible && (now_ms-t->hover_start)>=(uint64_t)t->show_delay_ms) { t->visible=1; return 1; }
    return 0;
}
int tooltip2_is_visible(const Tooltip2 *t) { return t?t->visible:-1; }
const char *tooltip2_text(const Tooltip2 *t) { return t?t->text:""; }
