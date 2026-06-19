/* puttyalt_seqrunner.c - Run an indexed sequence of steps.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SQ_MAX 64
typedef struct { int steps; int current; int failed_at; } SeqRunner;
void sq3_init(SeqRunner *r, int steps) {
    if (!r) return;
    r->steps = (steps > 0 && steps <= SQ_MAX) ? steps : 0;
    r->current = 0; r->failed_at = -1;
}
/* Advance; returns 1 if more steps remain, 0 if done, -1 if halted. */
int sq3_advance(SeqRunner *r, int step_ok) {
    if (!r || r->current >= r->steps) return 0;
    if (!step_ok) { r->failed_at = r->current; return -1; }
    r->current++;
    return r->current < r->steps ? 1 : 0;
}
int sq3_progress_pct(const SeqRunner *r) {
    if (!r || r->steps == 0) return 0;
    return r->current * 100 / r->steps;
}
int sq3_done(const SeqRunner *r) { return r && r->current >= r->steps; }
