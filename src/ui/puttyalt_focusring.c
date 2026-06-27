/* puttyalt_focusring.c - Manage focus traversal order.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define FR_MAX 32
typedef struct { int widgets[FR_MAX]; int n; int focused; } FocusRing;
void fr3_init(FocusRing *f) { if (f) { f->n = 0; f->focused = -1; } }
int fr3_add(FocusRing *f, int widget_id) {
    if (!f || f->n >= FR_MAX) return -1;
    f->widgets[f->n] = widget_id;
    if (f->focused < 0) f->focused = 0;
    return f->n++;
}
void fr3_next(FocusRing *f) { if (f && f->n) f->focused = (f->focused + 1) % f->n; }
void fr3_prev(FocusRing *f) { if (f && f->n) f->focused = (f->focused - 1 + f->n) % f->n; }
int fr3_current(const FocusRing *f) {
    return (f && f->focused >= 0 && f->focused < f->n) ? f->widgets[f->focused] : -1;
}
int fr3_focus_widget(FocusRing *f, int widget_id) {
    if (!f) return -1;
    for (int i = 0; i < f->n; i++) if (f->widgets[i] == widget_id) { f->focused = i; return 0; }
    return -1;
}
