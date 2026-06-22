/* puttyalt_dryrun.c - Track dry-run vs apply actions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define DR_MAX 64
typedef struct { char action[80]; int applied; } DrAction;
typedef struct { DrAction a[DR_MAX]; int n; int dry_run; } DryRun;
void dr3_init(DryRun *d, int dry_run) { if (d) { d->n = 0; d->dry_run = dry_run; } }
/* Record an action; returns 1 if it should actually execute. */
int dr3_record(DryRun *d, const char *action) {
    if (!d || !action) return 0;
    if (d->n < DR_MAX) {
        strncpy(d->a[d->n].action, action, 79); d->a[d->n].action[79] = 0;
        d->a[d->n].applied = d->dry_run ? 0 : 1;
        d->n++;
    }
    return d->dry_run ? 0 : 1;
}
int dr3_count(const DryRun *d) { return d ? d->n : 0; }
int dr3_applied_count(const DryRun *d) {
    if (!d) return 0;
    int c = 0;
    for (int i = 0; i < d->n; i++) if (d->a[i].applied) c++;
    return c;
}
