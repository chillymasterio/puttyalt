/* puttyalt_workflowval.c - Workflow validation (cycle/orphan detection). */
#include <string.h>
#define WV_MAX 32
typedef struct {
    int next[WV_MAX];
    int nnext;
    int visited;
    int in_stack;
} wv_node;
typedef struct {
    wv_node nodes[WV_MAX];
    int n;
} WorkflowVal;
void workflowval_init(WorkflowVal *w) {
    if (w) memset(w, 0, sizeof(*w));
}
int workflowval_add_node(WorkflowVal *w) {
    if (!w || w->n >= WV_MAX) return -1;
    return w->n++;
}
int workflowval_connect(WorkflowVal *w, int from, int to) {
    if (!w || from < 0 || from >= w->n || to < 0 || to >= w->n) return -1;
    if (w->nodes[from].nnext >= WV_MAX) return -1;
    w->nodes[from].next[w->nodes[from].nnext++] = to;
    return 0;
}
static int wv_has_cycle(WorkflowVal *w, int idx) {
    w->nodes[idx].visited = 1;
    w->nodes[idx].in_stack = 1;
    for (int i = 0; i < w->nodes[idx].nnext; i++) {
        int next = w->nodes[idx].next[i];
        if (w->nodes[next].in_stack) return 1;
        if (!w->nodes[next].visited && wv_has_cycle(w, next)) return 1;
    }
    w->nodes[idx].in_stack = 0;
    return 0;
}
int workflowval_has_cycle(WorkflowVal *w) {
    if (!w) return -1;
    for (int i = 0; i < w->n; i++) {
        w->nodes[i].visited = 0;
        w->nodes[i].in_stack = 0;
    }
    for (int i = 0; i < w->n; i++) {
        if (!w->nodes[i].visited && wv_has_cycle(w, i)) return 1;
    }
    return 0;
}
int workflowval_orphans(WorkflowVal *w, int *out_idx, int cap) {
    if (!w) return -1;
    int has_incoming[WV_MAX];
    memset(has_incoming, 0, sizeof(has_incoming));
    for (int i = 0; i < w->n; i++) {
        for (int j = 0; j < w->nodes[i].nnext; j++) {
            has_incoming[w->nodes[i].next[j]] = 1;
        }
    }
    int n = 0;
    for (int i = 1; i < w->n && n < cap; i++) {
        if (!has_incoming[i]) out_idx[n++] = i;
    }
    return n;
}
int workflowval_count(const WorkflowVal *w) {
    return w ? w->n : -1;
}
