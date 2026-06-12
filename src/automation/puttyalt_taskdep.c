/* puttyalt_taskdep.c - Task dependency graph with topological order. */
#include <string.h>
#include <stdio.h>
#define TD_MAX 32
#define TD_NAME 40
typedef struct {
    char name[TD_NAME];
    int deps[TD_MAX];
    int ndeps;
    int visited;
    int order;
} td_task;
typedef struct {
    td_task tasks[TD_MAX];
    int n;
} TaskDep;
void taskdep_init(TaskDep *t) {
    if (t) memset(t, 0, sizeof(*t));
}
int taskdep_add(TaskDep *t, const char *name) {
    if (!t || t->n >= TD_MAX || !name) return -1;
    snprintf(t->tasks[t->n].name, TD_NAME, "%s", name);
    return t->n++;
}
int taskdep_depend(TaskDep *t, int task, int depends_on) {
    if (!t || task < 0 || task >= t->n || depends_on < 0 || depends_on >= t->n) return -1;
    if (t->tasks[task].ndeps >= TD_MAX) return -1;
    t->tasks[task].deps[t->tasks[task].ndeps++] = depends_on;
    return 0;
}
static int td_visit(TaskDep *t, int idx, int *order, int *counter, int *cycle) {
    if (t->tasks[idx].visited == 2) return 0;
    if (t->tasks[idx].visited == 1) {
        *cycle = 1;
        return -1;
    }
    t->tasks[idx].visited = 1;
    for (int i = 0; i < t->tasks[idx].ndeps; i++)
        td_visit(t, t->tasks[idx].deps[i], order, counter, cycle);
    t->tasks[idx].visited = 2;
    order[(*counter)++] = idx;
    return 0;
}
int taskdep_topo_order(TaskDep *t, int *order, int cap) {
    if (!t || !order) return -1;
    for (int i = 0; i < t->n; i++) t->tasks[i].visited = 0;
    int counter = 0, cycle = 0;
    for (int i = 0; i < t->n && counter < cap; i++)
        td_visit(t, i, order, &counter, &cycle);
    return cycle ? -1 : counter;
}
int taskdep_count(const TaskDep *t) {
    return t ? t->n : -1;
}
