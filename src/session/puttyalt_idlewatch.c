/* puttyalt_idlewatch.c - Track session idle time and warnings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long last_activity; long idle_timeout; long warn_before; int warned; } IdleWatch;
void idw_init(IdleWatch *w, long timeout, long warn_before) {
    if (!w) return;
    w->idle_timeout=timeout>0?timeout:900;
    w->warn_before=warn_before;
    w->last_activity=0;
    w->warned=0;
}
void idw_activity(IdleWatch *w, long now) { if (w){ w->last_activity=now; w->warned=0; } }
int idw_should_warn(IdleWatch *w, long now) {
    if (!w||w->warned) return 0;
    if (now - w->last_activity >= w->idle_timeout - w->warn_before) { w->warned=1; return 1; }
    return 0;
}
int idw_expired(const IdleWatch *w, long now) { return w && (now - w->last_activity >= w->idle_timeout); }
