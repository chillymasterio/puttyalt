/* puttyalt_eventcounter.c - Count events per category with reset.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define EC_MAX 32
typedef struct { int category; long count; } EcEntry;
typedef struct { EcEntry e[EC_MAX]; int n; } EventCounter;
void ec2_init(EventCounter *c) { if (c) c->n = 0; }
void ec2_incr(EventCounter *c, int category) {
    if (!c) return;
    for (int i = 0; i < c->n; i++) if (c->e[i].category == category) { c->e[i].count++; return; }
    if (c->n < EC_MAX) { c->e[c->n].category = category; c->e[c->n].count = 1; c->n++; }
}
long ec2_get(const EventCounter *c, int category) {
    if (!c) return 0;
    for (int i = 0; i < c->n; i++) if (c->e[i].category == category) return c->e[i].count;
    return 0;
}
long ec2_total(const EventCounter *c) {
    if (!c) return 0;
    long t = 0;
    for (int i = 0; i < c->n; i++) t += c->e[i].count;
    return t;
}
void ec2_reset(EventCounter *c) { if (c) c->n = 0; }
