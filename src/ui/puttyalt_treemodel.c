/* puttyalt_treemodel.c - Generic expandable tree model.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TM_MAX 128
typedef struct { char label[40]; int parent; int expanded; int depth; } TmNode;
typedef struct { TmNode nodes[TM_MAX]; int n; } TreeModel;
void tm5_init(TreeModel *t) { if (t) t->n = 0; }
int tm5_add(TreeModel *t, int parent, const char *label) {
    if (!t || t->n >= TM_MAX || !label) return -1;
    int i = t->n++;
    strncpy(t->nodes[i].label, label, 39); t->nodes[i].label[39] = 0;
    t->nodes[i].parent = parent; t->nodes[i].expanded = 0;
    t->nodes[i].depth = (parent >= 0 && parent < i) ? t->nodes[parent].depth + 1 : 0;
    return i;
}
void tm5_toggle(TreeModel *t, int node) { if (t && node >= 0 && node < t->n) t->nodes[node].expanded = !t->nodes[node].expanded; }
/* Is a node visible (all ancestors expanded)? */
int tm5_visible(const TreeModel *t, int node) {
    if (!t || node < 0 || node >= t->n) return 0;
    int p = t->nodes[node].parent;
    while (p >= 0) {
        if (!t->nodes[p].expanded) return 0;
        p = t->nodes[p].parent;
    }
    return 1;
}
int tm5_visible_count(const TreeModel *t) {
    if (!t) return 0;
    int c = 0;
    for (int i = 0; i < t->n; i++) if (tm5_visible(t, i)) c++;
    return c;
}
