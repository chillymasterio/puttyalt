/* puttyalt_conntimer.c - Track per-session connection durations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define CT_MAX 32
typedef struct { int session_id; long start; long total; int active; } CtSession;
typedef struct { CtSession s[CT_MAX]; int n; } ConnTimer;
void ct3_init(ConnTimer *c) { if (c) c->n = 0; }
int ct3_start(ConnTimer *c, int session_id, long now) {
    if (!c) return -1;
    for (int i = 0; i < c->n; i++) if (c->s[i].session_id == session_id) { c->s[i].start = now; c->s[i].active = 1; return 0; }
    if (c->n >= CT_MAX) return -1;
    c->s[c->n].session_id = session_id; c->s[c->n].start = now; c->s[c->n].total = 0; c->s[c->n].active = 1;
    c->n++;
    return 0;
}
long ct3_stop(ConnTimer *c, int session_id, long now) {
    if (!c) return -1;
    for (int i = 0; i < c->n; i++)
        if (c->s[i].session_id == session_id && c->s[i].active) {
            long dur = now - c->s[i].start;
            c->s[i].total += dur; c->s[i].active = 0;
            return dur;
        }
    return -1;
}
long ct3_total(const ConnTimer *c, int session_id) {
    if (!c) return 0;
    for (int i = 0; i < c->n; i++) if (c->s[i].session_id == session_id) return c->s[i].total;
    return 0;
}
