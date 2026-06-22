/* puttyalt_bookmarktree.c - Hierarchical session bookmark folders.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define BT_MAX 128
typedef struct { char name[40]; int parent; int is_folder; int session_id; } BtNode;
typedef struct { BtNode nodes[BT_MAX]; int n; } BookmarkTree;
void bt2_init(BookmarkTree *t) { if (t) t->n = 0; }
int bt2_add_folder(BookmarkTree *t, int parent, const char *name) {
    if (!t || t->n >= BT_MAX || !name) return -1;
    int i = t->n++;
    strncpy(t->nodes[i].name, name, 39); t->nodes[i].name[39]=0;
    t->nodes[i].parent = parent; t->nodes[i].is_folder = 1; t->nodes[i].session_id = -1;
    return i;
}
int bt2_add_session(BookmarkTree *t, int parent, const char *name, int session_id) {
    if (!t || t->n >= BT_MAX || !name) return -1;
    int i = t->n++;
    strncpy(t->nodes[i].name, name, 39); t->nodes[i].name[39]=0;
    t->nodes[i].parent = parent; t->nodes[i].is_folder = 0; t->nodes[i].session_id = session_id;
    return i;
}
int bt2_children(const BookmarkTree *t, int parent, int *out, int cap) {
    if (!t) return 0;
    int c = 0;
    for (int i = 0; i < t->n && c < cap; i++) if (t->nodes[i].parent == parent) out[c++] = i;
    return c;
}
int bt2_depth(const BookmarkTree *t, int node) {
    if (!t || node < 0) return 0;
    int d = 0;
    while (node >= 0 && node < t->n && t->nodes[node].parent >= 0 && d < BT_MAX) { node = t->nodes[node].parent; d++; }
    return d;
}
