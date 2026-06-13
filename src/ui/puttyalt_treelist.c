/* puttyalt_treelist.c - Tree list view with expand/collapse + indent. */
#include <string.h>
#include <stdio.h>
#define TL_MAX 128
#define TL_LABEL 64
typedef struct {
    char label[TL_LABEL];
    int parent;
    int expanded;
    int has_children;
    int data_id;
} tl_node;
typedef struct {
    tl_node nodes[TL_MAX];
    int n;
    int selected;
} TreeList;
void treelist_init(TreeList *t) {
    if (!t) return;
    memset(t, 0, sizeof(*t));
    t->selected = -1;
}
int treelist_add(TreeList *t, const char *label, int parent, int data_id) {
    if (!t || t->n >= TL_MAX) return -1;
    if (parent >= t->n) return -1;
    tl_node *n = &t->nodes[t->n];
    snprintf(n->label, TL_LABEL, "%s", label ? label : "");
    n->parent = parent;
    n->data_id = data_id;
    if (parent >= 0) t->nodes[parent].has_children = 1;
    return t->n++;
}
void treelist_toggle(TreeList *t, int idx) {
    if (t && idx >= 0 && idx < t->n && t->nodes[idx].has_children) {
        t->nodes[idx].expanded ^= 1;
    }
}
int treelist_is_visible(const TreeList *t, int idx) {
    if (!t || idx < 0 || idx >= t->n) return 0;
    int p = t->nodes[idx].parent;
    int guard = 0;
    while (p >= 0 && guard++ < TL_MAX) {
        if (!t->nodes[p].expanded) return 0;
        p = t->nodes[p].parent;
    }
    return 1;
}
int treelist_depth(const TreeList *t, int idx) {
    if (!t || idx < 0 || idx >= t->n) return -1;
    int d = 0;
    int p = t->nodes[idx].parent;
    int guard = 0;
    while (p >= 0 && guard++ < TL_MAX) {
        d++;
        p = t->nodes[p].parent;
    }
    return d;
}
int treelist_visible_nodes(const TreeList *t, int *out_idx, int cap) {
    if (!t) return -1;
    int n = 0;
    for (int i = 0; i < t->n && n < cap; i++) {
        if (treelist_is_visible(t, i)) out_idx[n++] = i;
    }
    return n;
}
int treelist_count(const TreeList *t) {
    return t ? t->n : -1;
}
