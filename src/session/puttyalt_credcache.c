/* puttyalt_credcache.c - Cache credentials with TTL expiry.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CC_MAX 16
typedef struct { char host[64]; char user[48]; long expires; int used; } CcEntry;
typedef struct { CcEntry e[CC_MAX]; } CredCache;
void cc5_init(CredCache *c) { if (c) for (int i=0;i<CC_MAX;i++) c->e[i].used = 0; }
int cc5_store(CredCache *c, const char *host, const char *user, long now, long ttl) {
    if (!c || !host || !user) return -1;
    int slot = -1;
    for (int i = 0; i < CC_MAX; i++) {
        if (c->e[i].used && strcmp(c->e[i].host, host) == 0) { slot = i; break; }
        if (!c->e[i].used && slot < 0) slot = i;
    }
    if (slot < 0) return -1;
    strncpy(c->e[slot].host, host, 63); c->e[slot].host[63] = 0;
    strncpy(c->e[slot].user, user, 47); c->e[slot].user[47] = 0;
    c->e[slot].expires = now + ttl; c->e[slot].used = 1;
    return 0;
}
const char *cc5_get_user(CredCache *c, const char *host, long now) {
    if (!c || !host) return 0;
    for (int i = 0; i < CC_MAX; i++)
        if (c->e[i].used && strcmp(c->e[i].host, host) == 0) {
            if (now > c->e[i].expires) { c->e[i].used = 0; return 0; }
            return c->e[i].user;
        }
    return 0;
}
