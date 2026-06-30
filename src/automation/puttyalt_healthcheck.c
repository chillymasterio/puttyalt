/* puttyalt_healthcheck.c - Track component health states.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define HCK_MAX 16
enum { HEALTH_UNKNOWN, HEALTH_UP, HEALTH_DEGRADED, HEALTH_DOWN };
typedef struct { char name[24]; int state; long last_check; } HckComponent;
typedef struct { HckComponent c[HCK_MAX]; int n; } HealthCheck;
void hck_init(HealthCheck *h) { if (h) h->n = 0; }
void hck_report(HealthCheck *h, const char *name, int state, long now) {
    if (!h || !name) return;
    for (int i = 0; i < h->n; i++) if (!strcmp(h->c[i].name, name)) { h->c[i].state = state; h->c[i].last_check = now; return; }
    if (h->n < HCK_MAX) {
        strncpy(h->c[h->n].name, name, 23); h->c[h->n].name[23] = 0;
        h->c[h->n].state = state; h->c[h->n].last_check = now; h->n++;
    }
}
/* Overall: worst state across components. */
int hck_overall(const HealthCheck *h) {
    if (!h || h->n == 0) return HEALTH_UNKNOWN;
    int worst = HEALTH_UP;
    for (int i = 0; i < h->n; i++) if (h->c[i].state > worst) worst = h->c[i].state;
    return worst;
}
const char *hck_state_name(int state) {
    static const char *names[] = {"unknown","up","degraded","down"};
    return (state >= 0 && state <= HEALTH_DOWN) ? names[state] : "unknown";
}
