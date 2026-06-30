/* puttyalt_bellctl.c - Terminal bell rate limiting.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long last_bell; long min_interval; int count; int visual; } BellControl;
void bel_init(BellControl *b, long min_interval, int visual) {
    if (!b) return;
    b->last_bell = 0; b->min_interval = min_interval > 0 ? min_interval : 100;
    b->count = 0; b->visual = visual;
}
/* Returns 1 if the bell should sound (rate-limited). */
int bel_ring(BellControl *b, long now) {
    if (!b) return 0;
    b->count++;
    if (now - b->last_bell < b->min_interval) return 0;
    b->last_bell = now;
    return 1;
}
int bel_total(const BellControl *b) { return b ? b->count : 0; }
