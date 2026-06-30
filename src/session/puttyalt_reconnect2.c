/* puttyalt_reconnect2.c - Reconnection attempt scheduling.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int attempts; int max_attempts; long base_delay; long max_delay; } Reconnect2;
void rc4_init(Reconnect2 *r, int max_attempts, long base_delay, long max_delay) {
    if (!r) return;
    r->attempts = 0;
    r->max_attempts = max_attempts > 0 ? max_attempts : 10;
    r->base_delay = base_delay > 0 ? base_delay : 1;
    r->max_delay = max_delay > 0 ? max_delay : 60;
}
/* Returns delay for next attempt, or -1 if exhausted. */
long rc4_next_delay(Reconnect2 *r) {
    if (!r || r->attempts >= r->max_attempts) return -1;
    long delay = r->base_delay;
    for (int i = 0; i < r->attempts && delay < r->max_delay; i++) delay *= 2;
    if (delay > r->max_delay) delay = r->max_delay;
    r->attempts++;
    return delay;
}
void rc4_reset(Reconnect2 *r) { if (r) r->attempts = 0; }
int rc4_exhausted(const Reconnect2 *r) { return r && r->attempts >= r->max_attempts; }
