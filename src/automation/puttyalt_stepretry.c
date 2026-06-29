/* puttyalt_stepretry.c - Retry individual workflow steps with limits.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SR_MAX 32
typedef struct { int step_id; int attempts; int max_attempts; int succeeded; } SrStep;
typedef struct { SrStep steps[SR_MAX]; int n; } StepRetry;
void st4_init(StepRetry *r) { if (r) r->n = 0; }
int st4_register(StepRetry *r, int step_id, int max_attempts) {
    if (!r || r->n >= SR_MAX) return -1;
    r->steps[r->n].step_id = step_id;
    r->steps[r->n].attempts = 0;
    r->steps[r->n].max_attempts = max_attempts > 0 ? max_attempts : 3;
    r->steps[r->n].succeeded = 0;
    return r->n++;
}
/* Record an attempt result; returns 1 if should retry. */
int st4_attempt(StepRetry *r, int step_id, int success) {
    if (!r) return 0;
    for (int i = 0; i < r->n; i++) if (r->steps[i].step_id == step_id) {
        r->steps[i].attempts++;
        if (success) { r->steps[i].succeeded = 1; return 0; }
        return r->steps[i].attempts < r->steps[i].max_attempts;
    }
    return 0;
}
int st4_all_done(const StepRetry *r) {
    if (!r) return 1;
    for (int i = 0; i < r->n; i++)
        if (!r->steps[i].succeeded && r->steps[i].attempts >= r->steps[i].max_attempts) continue;
        else if (!r->steps[i].succeeded) return 0;
    return 1;
}
