/* puttyalt_schedrule.c - Evaluate recurring schedule rules.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { SR_DAILY, SR_WEEKLY, SR_HOURLY, SR_INTERVAL };
typedef struct { int type; int param; long last_run; long interval_sec; } SchedRule;
void sr3_init(SchedRule *r, int type, int param) {
    if (!r) return;
    r->type = type; r->param = param; r->last_run = 0;
    r->interval_sec = (type == SR_HOURLY) ? 3600 : (type == SR_DAILY) ? 86400 : (type == SR_WEEKLY) ? 604800 : param;
}
/* Should the rule fire at `now`? */
int sr3_due(SchedRule *r, long now) {
    if (!r) return 0;
    if (r->last_run == 0) return 1;
    return (now - r->last_run) >= r->interval_sec;
}
void sr3_mark_run(SchedRule *r, long now) { if (r) r->last_run = now; }
long sr3_next_run(const SchedRule *r) { return r ? r->last_run + r->interval_sec : 0; }
