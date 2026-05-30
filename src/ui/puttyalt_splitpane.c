/* puttyalt_splitpane.c - Binary tree of terminal panes (split H/V).
 * Self-contained library module for PuttyAlt (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SP_MAX_NODES 256
#define SP_NONE      (-1)

/* A node is either a leaf (a pane) or an internal split with two children. */
typedef struct {
    int    used;        /* slot occupied */
    int    leaf;        /* 1 = pane, 0 = internal split */
    int    parent;      /* index of parent node, SP_NONE for root */
    int    child[2];    /* child node indices (split nodes only) */
    int    vertical;    /* split orientation: 1 = side-by-side, 0 = stacked */
    float  ratio;       /* fraction of space given to child[0], 0..1 */
    int    pane_id;     /* stable id for leaves, SP_NONE otherwise */
    int    x, y, w, h;  /* computed pixel rectangle */
} SplitPaneNode;

typedef struct {
    SplitPaneNode nodes[SP_MAX_NODES];
    int           root;
    int           next_pane_id;
    int           win_w, win_h;
} SplitPaneTree;

int splitpane_layout(SplitPaneTree *t, int win_w, int win_h);

static int sp_alloc(SplitPaneTree *t) {
    for (int i = 0; i < SP_MAX_NODES; i++)
        if (!t->nodes[i].used) {
            memset(&t->nodes[i], 0, sizeof(t->nodes[i]));
            t->nodes[i].used = 1;
            t->nodes[i].parent = SP_NONE;
            t->nodes[i].child[0] = t->nodes[i].child[1] = SP_NONE;
            t->nodes[i].pane_id = SP_NONE;
            t->nodes[i].ratio = 0.5f;
            return i;
        }
    return SP_NONE;
}

static int sp_find_leaf(const SplitPaneTree *t, int pane_id) {
    for (int i = 0; i < SP_MAX_NODES; i++)
        if (t->nodes[i].used && t->nodes[i].leaf && t->nodes[i].pane_id == pane_id)
            return i;
    return SP_NONE;
}

void splitpane_init(SplitPaneTree *t, int win_w, int win_h) {
    if (!t) return;
    memset(t, 0, sizeof(*t));
    for (int i = 0; i < SP_MAX_NODES; i++) t->nodes[i].used = 0;
    t->win_w = win_w > 0 ? win_w : 0;
    t->win_h = win_h > 0 ? win_h : 0;
    t->next_pane_id = 0;
    t->root = sp_alloc(t);
    if (t->root != SP_NONE) {
        t->nodes[t->root].leaf = 1;
        t->nodes[t->root].pane_id = t->next_pane_id++;
    }
    splitpane_layout(t, win_w, win_h);
}

int splitpane_split(SplitPaneTree *t, int pane_id, int vertical, float ratio) {
    if (!t) return -1;
    int leaf = sp_find_leaf(t, pane_id);
    if (leaf == SP_NONE) return -1;
    if (ratio < 0.05f) ratio = 0.05f;
    if (ratio > 0.95f) ratio = 0.95f;
    int a = sp_alloc(t);
    int b = sp_alloc(t);
    if (a == SP_NONE || b == SP_NONE) {
        if (a != SP_NONE) t->nodes[a].used = 0;
        if (b != SP_NONE) t->nodes[b].used = 0;
        return -1;
    }
    /* a keeps the original pane_id; b is the new pane. */
    t->nodes[a].leaf = 1;
    t->nodes[a].parent = leaf;
    t->nodes[a].pane_id = pane_id;
    t->nodes[b].leaf = 1;
    t->nodes[b].parent = leaf;
    t->nodes[b].pane_id = t->next_pane_id++;
    /* The former leaf becomes an internal split node. */
    t->nodes[leaf].leaf = 0;
    t->nodes[leaf].pane_id = SP_NONE;
    t->nodes[leaf].vertical = vertical ? 1 : 0;
    t->nodes[leaf].ratio = ratio;
    t->nodes[leaf].child[0] = a;
    t->nodes[leaf].child[1] = b;
    splitpane_layout(t, t->win_w, t->win_h);
    return t->nodes[b].pane_id;
}

int splitpane_close(SplitPaneTree *t, int pane_id) {
    if (!t) return -1;
    int leaf = sp_find_leaf(t, pane_id);
    if (leaf == SP_NONE) return -1;
    int p = t->nodes[leaf].parent;
    if (p == SP_NONE) return -1;  /* cannot close the only pane */
    int sib = (t->nodes[p].child[0] == leaf) ? t->nodes[p].child[1]
                                             : t->nodes[p].child[0];
    if (sib == SP_NONE) return -1;
    t->nodes[leaf].used = 0;
    /* Collapse parent into the surviving sibling's subtree. */
    SplitPaneNode *gp = &t->nodes[p];
    int grand = gp->parent;
    t->nodes[sib].parent = grand;
    /* Copy sibling content into the parent slot to keep tree links stable. */
    SplitPaneNode keep = t->nodes[sib];
    keep.parent = grand;
    t->nodes[sib].used = 0;
    t->nodes[p] = keep;
    t->nodes[p].used = 1;
    if (!keep.leaf) {
        if (keep.child[0] != SP_NONE) t->nodes[keep.child[0]].parent = p;
        if (keep.child[1] != SP_NONE) t->nodes[keep.child[1]].parent = p;
    }
    splitpane_layout(t, t->win_w, t->win_h);
    return 0;
}

static void sp_layout_node(SplitPaneTree *t, int n, int x, int y, int w, int h) {
    if (n == SP_NONE || !t->nodes[n].used) return;
    SplitPaneNode *node = &t->nodes[n];
    node->x = x; node->y = y; node->w = w; node->h = h;
    if (node->leaf) return;
    if (node->vertical) {
        int w0 = (int)((float)w * node->ratio + 0.5f);
        if (w0 < 1) w0 = 1;
        if (w0 > w - 1) w0 = w - 1 > 0 ? w - 1 : w;
        sp_layout_node(t, node->child[0], x, y, w0, h);
        sp_layout_node(t, node->child[1], x + w0, y, w - w0, h);
    } else {
        int h0 = (int)((float)h * node->ratio + 0.5f);
        if (h0 < 1) h0 = 1;
        if (h0 > h - 1) h0 = h - 1 > 0 ? h - 1 : h;
        sp_layout_node(t, node->child[0], x, y, w, h0);
        sp_layout_node(t, node->child[1], x, y + h0, w, h - h0);
    }
}

int splitpane_layout(SplitPaneTree *t, int win_w, int win_h) {
    if (!t || t->root == SP_NONE) return -1;
    if (win_w < 0 || win_h < 0) return -1;
    t->win_w = win_w;
    t->win_h = win_h;
    sp_layout_node(t, t->root, 0, 0, win_w, win_h);
    return 0;
}

int splitpane_rect(const SplitPaneTree *t, int pane_id, int *x, int *y, int *w, int *h) {
    if (!t) return -1;
    int leaf = sp_find_leaf(t, pane_id);
    if (leaf == SP_NONE) return -1;
    const SplitPaneNode *node = &t->nodes[leaf];
    if (x) *x = node->x;
    if (y) *y = node->y;
    if (w) *w = node->w;
    if (h) *h = node->h;
    return 0;
}

int splitpane_count(const SplitPaneTree *t) {
    if (!t) return -1;
    int c = 0;
    for (int i = 0; i < SP_MAX_NODES; i++)
        if (t->nodes[i].used && t->nodes[i].leaf) c++;
    return c;
}
