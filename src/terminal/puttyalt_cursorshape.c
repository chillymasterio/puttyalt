/* puttyalt_cursorshape.c - Cursor shape/style (DECSCUSR) + blink. */
#include <string.h>
enum cs_shape { CS_BLOCK=0, CS_UNDERLINE=1, CS_BAR=2 };
typedef struct { int shape; int blink; int visible; unsigned int color; int color_set; } CursorShape;
void cursorshape_init(CursorShape *c) {
    if(!c) return;
    memset(c,0,sizeof(*c)); c->shape=CS_BLOCK; c->blink=1; c->visible=1;
}
int cursorshape_decscusr(CursorShape *c, int param) {
    if(!c) return -1;
    switch(param) {
        case 0: case 1: c->shape=CS_BLOCK; c->blink=1; break;
        case 2: c->shape=CS_BLOCK; c->blink=0; break;
        case 3: c->shape=CS_UNDERLINE; c->blink=1; break;
        case 4: c->shape=CS_UNDERLINE; c->blink=0; break;
        case 5: c->shape=CS_BAR; c->blink=1; break;
        case 6: c->shape=CS_BAR; c->blink=0; break;
        default: return -1;
    }
    return 0;
}
void cursorshape_set_visible(CursorShape *c, int v) { if(c) c->visible=v?1:0; }
void cursorshape_set_color(CursorShape *c, unsigned int color) { if(c){ c->color=color; c->color_set=1; } }
int cursorshape_get(const CursorShape *c) { return c?c->shape:-1; }
int cursorshape_blinks(const CursorShape *c) { return c?c->blink:-1; }
