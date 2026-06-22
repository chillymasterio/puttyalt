/* puttyalt_panelresize.c - Resize split panels with constraints.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int size_a; int size_b; int min_a; int min_b; int total; } PanelResize;
void pr2_init(PanelResize *p, int total, int min_a, int min_b) {
    if (!p) return;
    p->total = total; p->min_a = min_a; p->min_b = min_b;
    p->size_a = total / 2; p->size_b = total - p->size_a;
}
/* Drag the divider by `delta`, respecting minimums. */
void pr2_drag(PanelResize *p, int delta) {
    if (!p) return;
    int na = p->size_a + delta;
    int nb = p->total - na;
    if (na < p->min_a) { na = p->min_a; nb = p->total - na; }
    if (nb < p->min_b) { nb = p->min_b; na = p->total - nb; }
    p->size_a = na; p->size_b = nb;
}
/* Re-distribute on total change keeping the ratio. */
void pr2_set_total(PanelResize *p, int total) {
    if (!p || p->total <= 0) return;
    long ratio = (long)p->size_a * 1000 / p->total;
    p->total = total;
    p->size_a = (int)(total * ratio / 1000);
    p->size_b = total - p->size_a;
    if (p->size_a < p->min_a) { p->size_a = p->min_a; p->size_b = total - p->size_a; }
}
