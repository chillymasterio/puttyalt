/* puttyalt_profilemerge.c - Merge connection profiles with precedence.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct {
    char host[96]; int port; char user[48];
    int has_host, has_port, has_user;
} Profile;
void pm4_clear(Profile *p) { if (p) { p->has_host = p->has_port = p->has_user = 0; p->port = 0; p->host[0]=p->user[0]=0; } }
/* Apply `over` fields onto `base` where set. */
void pm4_apply(Profile *base, const Profile *over) {
    if (!base || !over) return;
    if (over->has_host) { strncpy(base->host, over->host, 95); base->host[95]=0; base->has_host = 1; }
    if (over->has_port) { base->port = over->port; base->has_port = 1; }
    if (over->has_user) { strncpy(base->user, over->user, 47); base->user[47]=0; base->has_user = 1; }
}
int pm4_complete(const Profile *p) { return p && p->has_host && p->has_port && p->has_user; }
