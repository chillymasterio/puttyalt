/* puttyalt_authlog.c - Authentication attempt log with lockout detection. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define AL_MAX 64
#define AL_USER 48
typedef struct { char user[AL_USER]; int failures; uint64_t last_fail_ms; int locked; } al_entry;
typedef struct { al_entry e[AL_MAX]; int n; int max_failures; int lockout_ms; } AuthLog;
void authlog_init(AuthLog *a, int max_failures, int lockout_ms) {
    if(!a) return; memset(a,0,sizeof(*a)); a->max_failures=max_failures>0?max_failures:5; a->lockout_ms=lockout_ms>0?lockout_ms:300000;
}
static al_entry *al_find(AuthLog *a, const char *user) {
    for (int i=0;i<a->n;i++) if (strcmp(a->e[i].user,user)==0) return &a->e[i];
    if (a->n>=AL_MAX) return 0;
    al_entry *e=&a->e[a->n++]; snprintf(e->user,AL_USER,"%s",user); return e;
}
int authlog_record(AuthLog *a, const char *user, int success, uint64_t now_ms) {
    if(!a||!user) return -1; al_entry *e=al_find(a,user); if(!e) return -1;
    if (success) { e->failures=0; e->locked=0; return 0; }
    e->failures++; e->last_fail_ms=now_ms;
    if (e->failures>=a->max_failures) { e->locked=1; return 1; } /* now locked */
    return 0;
}
int authlog_is_locked(AuthLog *a, const char *user, uint64_t now_ms) {
    if(!a||!user) return 0;
    for (int i=0;i<a->n;i++) if (strcmp(a->e[i].user,user)==0) {
        if (a->e[i].locked && (now_ms-a->e[i].last_fail_ms)>(uint64_t)a->lockout_ms) { a->e[i].locked=0; a->e[i].failures=0; }
        return a->e[i].locked;
    }
    return 0;
}
int authlog_failures(const AuthLog *a, const char *user) {
    if(!a||!user) return -1;
    for (int i=0;i<a->n;i++) if (strcmp(a->e[i].user,user)==0) return a->e[i].failures;
    return 0;
}
