/* puttyalt_scriptloop.c - Bounded loop control for scripts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int iterations; int max_iterations; int broken; } ScriptLoop;
void sl4_init(ScriptLoop *l, int max_iterations) {
    if (!l) return;
    l->iterations = 0;
    l->max_iterations = max_iterations > 0 ? max_iterations : 10000;
    l->broken = 0;
}
/* Returns 1 if the loop may continue, 0 if it should stop. */
int sl4_continue(ScriptLoop *l) {
    if (!l || l->broken) return 0;
    if (l->iterations >= l->max_iterations) return 0;
    l->iterations++;
    return 1;
}
void sl4_break(ScriptLoop *l) { if (l) l->broken = 1; }
int sl4_count(const ScriptLoop *l) { return l ? l->iterations : 0; }
int sl4_exhausted(const ScriptLoop *l) { return l && l->iterations >= l->max_iterations; }
