/* puttyalt_contextmenu2.c - Build and navigate context menus.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CX_MAX 24
typedef struct { char label[40]; int id; int enabled; int separator; } CxItem;
typedef struct { CxItem items[CX_MAX]; int n; int hover; } ContextMenu2;
void cx2_init(ContextMenu2 *m) { if (m) { m->n = 0; m->hover = -1; } }
int cx2_add(ContextMenu2 *m, const char *label, int id) {
    if (!m || m->n >= CX_MAX) return -1;
    strncpy(m->items[m->n].label, label ? label : "", 39); m->items[m->n].label[39]=0;
    m->items[m->n].id = id; m->items[m->n].enabled = 1; m->items[m->n].separator = 0;
    return m->n++;
}
int cx2_separator(ContextMenu2 *m) {
    if (!m || m->n >= CX_MAX) return -1;
    m->items[m->n].separator = 1; m->items[m->n].enabled = 0; m->items[m->n].label[0]=0;
    return m->n++;
}
/* Move hover to next/prev enabled non-separator item. */
void cx2_navigate(ContextMenu2 *m, int dir) {
    if (!m || m->n == 0) return;
    for (int step = 0; step < m->n; step++) {
        m->hover = (m->hover + dir + m->n) % m->n;
        if (m->items[m->hover].enabled && !m->items[m->hover].separator) return;
    }
}
int cx2_selected_id(const ContextMenu2 *m) {
    return (m && m->hover >= 0 && m->hover < m->n) ? m->items[m->hover].id : -1;
}
