/* puttyalt_alertdedup.c - Deduplicate alerts within a window.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define AD_MAX 32
typedef struct { char key[48]; long last_fired; int count; } AdAlert;
typedef struct { AdAlert a[AD_MAX]; int n; long suppress_window; } AlertDedup;
void ad3_init(AlertDedup *d, long suppress_window) {
    if (!d) return;
    d->n = 0; d->suppress_window = suppress_window > 0 ? suppress_window : 300;
}
/* Returns 1 if the alert should fire (not suppressed). */
int ad3_should_fire(AlertDedup *d, const char *key, long now) {
    if (!d || !key) return 0;
    for (int i = 0; i < d->n; i++) if (!strcmp(d->a[i].key, key)) {
        d->a[i].count++;
        if (now - d->a[i].last_fired < d->suppress_window) return 0;
        d->a[i].last_fired = now;
        return 1;
    }
    if (d->n < AD_MAX) {
        strncpy(d->a[d->n].key, key, 47); d->a[d->n].key[47] = 0;
        d->a[d->n].last_fired = now; d->a[d->n].count = 1; d->n++;
    }
    return 1;
}
