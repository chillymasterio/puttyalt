/* puttyalt_retrybudget.c - Limit total retries within a budget.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int budget; int used; long window_start; long window; } RetryBudget;
void rb2_init(RetryBudget *b, int budget, long window) {
    if (!b) return;
    b->budget = budget > 0 ? budget : 10;
    b->window = window > 0 ? window : 60;
    b->used = 0; b->window_start = 0;
}
/* Try to spend one retry at `now`. Returns 1 if allowed. */
int rb2_try(RetryBudget *b, long now) {
    if (!b) return 0;
    if (now - b->window_start >= b->window) { b->window_start = now; b->used = 0; }
    if (b->used >= b->budget) return 0;
    b->used++;
    return 1;
}
int rb2_remaining(const RetryBudget *b) { return b ? b->budget - b->used : 0; }
