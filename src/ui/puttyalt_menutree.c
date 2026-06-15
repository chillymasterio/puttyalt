/* puttyalt_menutree.c - Hierarchical menu model.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MT_MAX 128
typedef struct { char label[40]; int parent; int enabled; int id; } MtItem;
typedef struct { MtItem item[MT_MAX]; int n; } MenuTree;
void mt2_init(MenuTree *m) { if (m) m->n = 0; }
int mt2_add(MenuTree *m, int parent, const char *label, int id) {
    if (!m || m->n >= MT_MAX || !label) return -1;
    int i = m->n++;
    strncpy(m->item[i].label, label, 39); m->item[i].label[39] = 0;
    m->item[i].parent = parent; m->item[i].enabled = 1; m->item[i].id = id;
    return i;
}
int mt2_children(const MenuTree *m, int parent, int *out, int outcap) {
    if (!m) return 0;
    int c = 0;
    for (int i = 0; i < m->n && c < outcap; i++) if (m->item[i].parent == parent) out[c++] = i;
    return c;
}
void mt2_enable(MenuTree *m, int idx, int on) { if (m && idx>=0 && idx<m->n) m->item[idx].enabled = on?1:0; }
