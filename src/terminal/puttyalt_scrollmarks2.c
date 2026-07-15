/* puttyalt_scrollmarks2.c - Track scroll position markers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SM_MAX 16
typedef struct { int lines[SM_MAX]; int n; } ScrollMarks;
void smk_init(ScrollMarks *m) { if (m) m->n = 0; }
int smk_add(ScrollMarks *m, int line) {
    if (!m || m->n >= SM_MAX) return -1;
    m->lines[m->n++] = line; return 0;
}
int smk_next(const ScrollMarks *m, int from) {
    if (!m) return -1;
    int best = -1;
    for (int i=0;i<m->n;i++) if (m->lines[i] > from && (best<0 || m->lines[i]<best)) best = m->lines[i];
    return best;
}
int smk_prev(const ScrollMarks *m, int from) {
    if (!m) return -1;
    int best = -1;
    for (int i=0;i<m->n;i++) if (m->lines[i] < from && m->lines[i] > best) best = m->lines[i];
    return best;
}
