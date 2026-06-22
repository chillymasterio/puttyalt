/* puttyalt_jobgraph.c - Topological order of dependent jobs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define JG_MAX 32
typedef struct { int deps[JG_MAX]; int ndeps; int done; } JgJob;
typedef struct { JgJob jobs[JG_MAX]; int n; } JobGraph;
void jg2_init(JobGraph *g, int n) {
    if (!g) return;
    g->n = (n > 0 && n <= JG_MAX) ? n : 0;
    for (int i = 0; i < g->n; i++) { g->jobs[i].ndeps = 0; g->jobs[i].done = 0; }
}
int jg2_depend(JobGraph *g, int job, int dep) {
    if (!g || job < 0 || job >= g->n || dep < 0 || dep >= g->n) return -1;
    if (g->jobs[job].ndeps >= JG_MAX) return -1;
    g->jobs[job].deps[g->jobs[job].ndeps++] = dep;
    return 0;
}
/* Return next runnable job (all deps done, not itself done), or -1. */
int jg2_next(JobGraph *g) {
    if (!g) return -1;
    for (int i = 0; i < g->n; i++) {
        if (g->jobs[i].done) continue;
        int ready = 1;
        for (int d = 0; d < g->jobs[i].ndeps; d++)
            if (!g->jobs[g->jobs[i].deps[d]].done) { ready = 0; break; }
        if (ready) return i;
    }
    return -1;
}
void jg2_complete(JobGraph *g, int job) { if (g && job >= 0 && job < g->n) g->jobs[job].done = 1; }
int jg2_all_done(const JobGraph *g) {
    if (!g) return 1;
    for (int i = 0; i < g->n; i++) if (!g->jobs[i].done) return 0;
    return 1;
}
