/* puttyalt_scheduler2.c - Priority task scheduler (earliest deadline).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SCH_CAP 64
typedef struct { int id; long deadline; int active; } SchTask;
typedef struct { SchTask t[SCH_CAP]; int n; } Scheduler2;
void sch2_init(Scheduler2 *s) { if (s) s->n = 0; }
int sch2_add(Scheduler2 *s, int id, long deadline) {
    if (!s || s->n >= SCH_CAP) return -1;
    s->t[s->n].id = id; s->t[s->n].deadline = deadline; s->t[s->n].active = 1; s->n++;
    return 0;
}
/* Return id of the earliest-deadline active task, or -1. */
int sch2_next(const Scheduler2 *s, long *deadline_out) {
    if (!s) return -1;
    int best = -1; long bd = 0;
    for (int i = 0; i < s->n; i++)
        if (s->t[i].active && (best < 0 || s->t[i].deadline < bd)) { best = i; bd = s->t[i].deadline; }
    if (best < 0) return -1;
    if (deadline_out) *deadline_out = bd;
    return s->t[best].id;
}
int sch2_complete(Scheduler2 *s, int id) {
    if (!s) return -1;
    for (int i = 0; i < s->n; i++) if (s->t[i].active && s->t[i].id == id) { s->t[i].active = 0; return 0; }
    return -1;
}
