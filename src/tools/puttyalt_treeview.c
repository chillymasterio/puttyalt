/* puttyalt_treeview.c - Generic tree view model with expand/select/flatten. */
#include <string.h>
#include <stdio.h>
#define TV_MAX 128
#define TV_LABEL 64
typedef struct { char label[TV_LABEL]; int parent; int expanded; int has_children; } tv_node;
typedef struct { tv_node n[TV_MAX]; int count; int selected; } TreeView;
void treeview_init(TreeView *t) { if(t){ memset(t,0,sizeof(*t)); t->selected=-1; } }
int treeview_add(TreeView *t, const char *label, int parent) {
    if(!t||t->count>=TV_MAX||!label) return -1;
    if (parent>=t->count) return -1;
    tv_node *n=&t->n[t->count]; snprintf(n->label,TV_LABEL,"%s",label); n->parent=parent;
    if (parent>=0) t->n[parent].has_children=1;
    return t->count++;
}
void treeview_toggle(TreeView *t, int idx) { if(t&&idx>=0&&idx<t->count&&t->n[idx].has_children) t->n[idx].expanded^=1; }
int treeview_is_visible(const TreeView *t, int idx) {
    if(!t||idx<0||idx>=t->count) return 0;
    int p=t->n[idx].parent, guard=0;
    while (p>=0 && guard++<TV_MAX) { if (!t->n[p].expanded) return 0; p=t->n[p].parent; }
    return 1;
}
int treeview_flatten(const TreeView *t, int *out_idx, int cap) {
    if(!t) return -1; int n=0;
    for (int i=0;i<t->count && n<cap;i++) if (treeview_is_visible(t,i)) out_idx[n++]=i;
    return n;
}
int treeview_count(const TreeView *t) { return t?t->count:-1; }
