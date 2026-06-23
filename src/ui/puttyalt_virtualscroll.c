/* puttyalt_virtualscroll.c - Virtual scrolling window calculations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int total_items; int item_height; int viewport_height; int scroll_top; } VirtualScroll;
void vsc_init(VirtualScroll *v, int item_height, int viewport_height) {
    if (!v) return;
    v->total_items = 0; v->item_height = item_height > 0 ? item_height : 1;
    v->viewport_height = viewport_height; v->scroll_top = 0;
}
int vsc_first_visible(const VirtualScroll *v) { return v ? v->scroll_top / v->item_height : 0; }
int vsc_visible_count(const VirtualScroll *v) {
    if (!v) return 0;
    return v->viewport_height / v->item_height + 2;
}
int vsc_total_height(const VirtualScroll *v) { return v ? v->total_items * v->item_height : 0; }
int vsc_max_scroll(const VirtualScroll *v) {
    if (!v) return 0;
    int h = vsc_total_height(v) - v->viewport_height;
    return h > 0 ? h : 0;
}
