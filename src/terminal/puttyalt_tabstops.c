/* puttyalt_tabstops.c - Terminal tab-stop tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TAB_MAX 512
typedef struct { unsigned char stop[TAB_MAX]; int width; } TabStops;
void tab_init(TabStops *t, int width) {
    if (!t) return;
    memset(t->stop, 0, sizeof t->stop);
    t->width = (width > 0 && width <= TAB_MAX) ? width : 80;
    for (int i = 0; i < t->width; i += 8) t->stop[i] = 1;
}
void tab_set(TabStops *t, int col) { if (t && col >= 0 && col < TAB_MAX) t->stop[col] = 1; }
void tab_clear(TabStops *t, int col) { if (t && col >= 0 && col < TAB_MAX) t->stop[col] = 0; }
void tab_clear_all(TabStops *t) { if (t) memset(t->stop, 0, sizeof t->stop); }
int tab_next(const TabStops *t, int col) {
    if (!t) return col + 1;
    for (int c = col + 1; c < t->width; c++) if (t->stop[c]) return c;
    return t->width - 1;
}
int tab_prev(const TabStops *t, int col) {
    if (!t) return col > 0 ? col - 1 : 0;
    for (int c = col - 1; c > 0; c--) if (t->stop[c]) return c;
    return 0;
}
