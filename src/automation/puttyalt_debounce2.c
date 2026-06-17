/* puttyalt_debounce2.c - Debounce rapidly repeated events.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long last_event; long quiet; long pending_since; int has_pending; } Debouncer;
void db2_init(Debouncer *d, long quiet_period) {
    if (!d) return;
    d->quiet = quiet_period > 0 ? quiet_period : 100;
    d->last_event = 0; d->pending_since = 0; d->has_pending = 0;
}
/* Register an event at `now`; it will fire after the quiet period. */
void db2_event(Debouncer *d, long now) {
    if (!d) return;
    if (!d->has_pending) { d->pending_since = now; d->has_pending = 1; }
    d->last_event = now;
}
/* Returns 1 if a debounced event should fire now. */
int db2_ready(Debouncer *d, long now) {
    if (!d || !d->has_pending) return 0;
    if (now - d->last_event >= d->quiet) { d->has_pending = 0; return 1; }
    return 0;
}
