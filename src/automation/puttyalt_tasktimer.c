/* puttyalt_tasktimer.c - Measure and report task execution time.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define TT_MAX 32
typedef struct { char name[32]; long start; long elapsed; int running; } TtTask;
typedef struct { TtTask tasks[TT_MAX]; int n; } TaskTimer;
void tt3_init(TaskTimer *t) { if (t) t->n = 0; }
static TtTask *tt3_find(TaskTimer *t, const char *name) {
    for (int i = 0; i < t->n; i++) {
        const char *a = t->tasks[i].name, *b = name;
        while (*a && *b && *a == *b) { a++; b++; }
        if (!*a && !*b) return &t->tasks[i];
    }
    return 0;
}
int tt3_start(TaskTimer *t, const char *name, long now) {
    if (!t || !name) return -1;
    TtTask *task = tt3_find(t, name);
    if (!task) {
        if (t->n >= TT_MAX) return -1;
        task = &t->tasks[t->n++];
        int i = 0; while (name[i] && i < 31) { task->name[i] = name[i]; i++; } task->name[i] = 0;
        task->elapsed = 0;
    }
    task->start = now; task->running = 1;
    return 0;
}
long tt3_stop(TaskTimer *t, const char *name, long now) {
    TtTask *task = tt3_find(t, name);
    if (!task || !task->running) return -1;
    task->elapsed += now - task->start;
    task->running = 0;
    return task->elapsed;
}
