/* puttyalt_sessclone.c - Clone session config with overrides.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct {
    char name[48]; char host[64]; int port;
    char user[32]; char keyfile[128]; int color;
} SessionCfg;
void scl_copy(const SessionCfg *src, SessionCfg *dst) { if (src && dst) *dst = *src; }
int scl_clone_named(const SessionCfg *src, const char *new_name, SessionCfg *dst) {
    if (!src || !dst || !new_name) return -1;
    *dst = *src;
    strncpy(dst->name, new_name, sizeof(dst->name)-1);
    dst->name[sizeof(dst->name)-1] = 0;
    return 0;
}
int scl_differs(const SessionCfg *a, const SessionCfg *b) {
    if (!a || !b) return 1;
    return strcmp(a->host,b->host) || a->port != b->port ||
           strcmp(a->user,b->user) || strcmp(a->keyfile,b->keyfile);
}
