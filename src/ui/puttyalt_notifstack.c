/* puttyalt_notifstack.c - Stack and expire toast notifications.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define NS_MAX 8
typedef struct { char text[96]; long expires; int level; int used; } NsToast;
typedef struct { NsToast t[NS_MAX]; } NotifStack;
void ns3_init(NotifStack *s) { if (s) for (int i=0;i<NS_MAX;i++) s->t[i].used = 0; }
int ns3_push(NotifStack *s, const char *text, int level, long now, long ttl) {
    if (!s || !text) return -1;
    for (int i = 0; i < NS_MAX; i++) if (!s->t[i].used) {
        strncpy(s->t[i].text, text, 95); s->t[i].text[95]=0;
        s->t[i].level = level; s->t[i].expires = now + ttl; s->t[i].used = 1;
        return i;
    }
    return -1;
}
/* Expire stale toasts; returns active count. */
int ns3_tick(NotifStack *s, long now) {
    if (!s) return 0;
    int active = 0;
    for (int i = 0; i < NS_MAX; i++) {
        if (s->t[i].used && now >= s->t[i].expires) s->t[i].used = 0;
        if (s->t[i].used) active++;
    }
    return active;
}
