/* puttyalt_workflowdag.c - Validate workflow DAG for cycles.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define WD_MAX 32
typedef struct { int edges[WD_MAX]; int nedges; } WdNode;
typedef struct { WdNode nodes[WD_MAX]; int n; } WorkflowDag;
void wdg_init(WorkflowDag *g, int n) {
    if (!g) return;
    g->n = (n > 0 && n <= WD_MAX) ? n : 0;
    for (int i = 0; i < g->n; i++) g->nodes[i].nedges = 0;
}
int wdg_edge(WorkflowDag *g, int from, int to) {
    if (!g || from < 0 || from >= g->n || to < 0 || to >= g->n) return -1;
    if (g->nodes[from].nedges >= WD_MAX) return -1;
    g->nodes[from].edges[g->nodes[from].nedges++] = to;
    return 0;
}
static int wdg_visit(const WorkflowDag *g, int node, int *state) {
    state[node] = 1; /* in progress */
    for (int i = 0; i < g->nodes[node].nedges; i++) {
        int next = g->nodes[node].edges[i];
        if (state[next] == 1) return 1; /* back edge = cycle */
        if (state[next] == 0 && wdg_visit(g, next, state)) return 1;
    }
    state[node] = 2; /* done */
    return 0;
}
/* Returns 1 if the DAG has a cycle. */
int wdg_has_cycle(const WorkflowDag *g) {
    if (!g) return 0;
    int state[WD_MAX];
    memset(state, 0, sizeof state);
    for (int i = 0; i < g->n; i++) if (state[i] == 0 && wdg_visit(g, i, state)) return 1;
    return 0;
}
