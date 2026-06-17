/* puttyalt_watchdog2.c - Heartbeat watchdog timeout tracker.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long last_beat; long timeout; int armed; int triggered; } Watchdog2;
void wd2_init(Watchdog2 *w, long timeout) {
    if (!w) return;
    w->timeout = timeout > 0 ? timeout : 30;
    w->last_beat = 0; w->armed = 0; w->triggered = 0;
}
void wd2_arm(Watchdog2 *w, long now) { if (w) { w->armed = 1; w->last_beat = now; w->triggered = 0; } }
void wd2_beat(Watchdog2 *w, long now) { if (w) { w->last_beat = now; w->triggered = 0; } }
/* Returns 1 if the watchdog just fired. */
int wd2_check(Watchdog2 *w, long now) {
    if (!w || !w->armed) return 0;
    if (!w->triggered && now - w->last_beat >= w->timeout) { w->triggered = 1; return 1; }
    return 0;
}
