/* puttyalt_quickswitch2.c - MRU quick-switch ordering.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define QS_MAX 32
typedef struct { int order[QS_MAX]; int n; } QuickSwitch2;
void qs3_init(QuickSwitch2 *q) { if (q) q->n = 0; }
/* Touch a tab: move it to the front of the MRU list. */
void qs3_touch(QuickSwitch2 *q, int tab_id) {
    if (!q) return;
    int pos = -1;
    for (int i = 0; i < q->n; i++) if (q->order[i] == tab_id) { pos = i; break; }
    if (pos >= 0) for (int i = pos; i > 0; i--) q->order[i] = q->order[i-1];
    else { if (q->n < QS_MAX) q->n++; for (int i = q->n-1; i > 0; i--) q->order[i] = q->order[i-1]; }
    q->order[0] = tab_id;
}
/* Get the Nth most-recent tab (1 = previous). */
int qs3_mru(const QuickSwitch2 *q, int n) {
    if (!q || n < 0 || n >= q->n) return -1;
    return q->order[n];
}
