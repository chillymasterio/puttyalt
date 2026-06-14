/* puttyalt_cooldown.c - Per-key cooldown / debounce gate.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CD_MAX 64
typedef struct { int key; long next_allowed; } CdEntry;
typedef struct { CdEntry e[CD_MAX]; int n; long interval; } CdGate;
void cd_init(CdGate *g, long interval) { if (g) { g->n = 0; g->interval = interval > 0 ? interval : 1; } }
/* Returns 1 if action allowed at time `now`, else 0. Updates state. */
int cd_check(CdGate *g, int key, long now) {
    if (!g) return 0;
    for (int i = 0; i < g->n; i++) {
        if (g->e[i].key == key) {
            if (now < g->e[i].next_allowed) return 0;
            g->e[i].next_allowed = now + g->interval;
            return 1;
        }
    }
    if (g->n < CD_MAX) { g->e[g->n].key = key; g->e[g->n].next_allowed = now + g->interval; g->n++; }
    return 1;
}
