/* puttyalt_listview2.c - List view with selection and scroll.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int count; int selected; int top; int visible; } ListView2;
void lv3_init(ListView2 *l, int visible) {
    if (!l) return;
    l->count = 0;
    l->selected = 0;
    l->top = 0;
    l->visible = visible > 0 ? visible : 10;
}
void lv3_set_count(ListView2 *l, int count) {
    if (!l) return;
    l->count = count < 0 ? 0 : count;
    if (l->selected >= l->count) l->selected = l->count - 1;
    if (l->selected < 0) l->selected = 0;
}
void lv3_move(ListView2 *l, int delta) {
    if (!l || l->count == 0) return;
    l->selected += delta;
    if (l->selected < 0) l->selected = 0;
    if (l->selected >= l->count) l->selected = l->count - 1;
    if (l->selected < l->top) l->top = l->selected;
    if (l->selected >= l->top + l->visible) l->top = l->selected - l->visible + 1;
}
int lv3_is_visible(const ListView2 *l, int index) {
    return l && index >= l->top && index < l->top + l->visible;
}
