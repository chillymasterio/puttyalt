/* puttyalt_ratelimitauth.c - Lock out auth after failed attempts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define RLA_MAX 32
typedef struct { char user[48]; int fails; long locked_until; int used; } RlaEntry;
typedef struct { RlaEntry e[RLA_MAX]; int threshold; long lockout; } AuthLimiter;
void rla_init(AuthLimiter *a, int threshold, long lockout) {
    if (!a) return;
    for (int i=0;i<RLA_MAX;i++) a->e[i].used=0;
    a->threshold = threshold > 0 ? threshold : 5;
    a->lockout = lockout > 0 ? lockout : 300;
}
static RlaEntry *rla_find(AuthLimiter *a, const char *user) {
    int slot=-1;
    for (int i=0;i<RLA_MAX;i++) {
        if (a->e[i].used && strcmp(a->e[i].user,user)==0) return &a->e[i];
        if (!a->e[i].used && slot<0) slot=i;
    }
    if (slot<0) return 0;
    strncpy(a->e[slot].user, user, 47); a->e[slot].user[47]=0;
    a->e[slot].fails=0; a->e[slot].locked_until=0; a->e[slot].used=1;
    return &a->e[slot];
}
int rla_is_locked(AuthLimiter *a, const char *user, long now) {
    RlaEntry *e = rla_find(a, user);
    return e && now < e->locked_until;
}
void rla_fail(AuthLimiter *a, const char *user, long now) {
    RlaEntry *e = rla_find(a, user);
    if (!e) return;
    e->fails++;
    if (e->fails >= a->threshold) { e->locked_until = now + a->lockout; e->fails = 0; }
}
void rla_success(AuthLimiter *a, const char *user) {
    RlaEntry *e = rla_find(a, user);
    if (e) { e->fails = 0; e->locked_until = 0; }
}
