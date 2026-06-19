/* puttyalt_tabstrip.c - Manage tab strip ordering and active tab.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define TST_MAX 32
typedef struct { int order[TST_MAX]; int n; int active; } TabStrip;
void tst_init(TabStrip *t) { if (t) { t->n = 0; t->active = -1; } }
int tst_add(TabStrip *t, int tab_id) {
    if (!t || t->n >= TST_MAX) return -1;
    t->order[t->n] = tab_id;
    if (t->active < 0) t->active = 0;
    return t->n++;
}
int tst_move(TabStrip *t, int from, int to) {
    if (!t || from < 0 || from >= t->n || to < 0 || to >= t->n) return -1;
    int v = t->order[from];
    if (from < to) for (int i = from; i < to; i++) t->order[i] = t->order[i+1];
    else for (int i = from; i > to; i--) t->order[i] = t->order[i-1];
    t->order[to] = v;
    return 0;
}
void tst_next(TabStrip *t) { if (t && t->n) t->active = (t->active + 1) % t->n; }
void tst_prev(TabStrip *t) { if (t && t->n) t->active = (t->active - 1 + t->n) % t->n; }
int tst_active_id(const TabStrip *t) { return (t && t->active >= 0 && t->active < t->n) ? t->order[t->active] : -1; }
