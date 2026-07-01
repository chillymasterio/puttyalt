/* puttyalt_statusflash.c - Timed status message flashing.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { char msg[96]; long expires; int level; } StatusFlash;
void sfl2_init(StatusFlash *s) { if (s) { s->msg[0]=0; s->expires=0; s->level=0; } }
void sfl2_show(StatusFlash *s, const char *msg, int level, long now, long ttl) {
    if (!s || !msg) return;
    strncpy(s->msg, msg, 95); s->msg[95]=0; s->level=level; s->expires=now+ttl;
}
const char *sfl2_current(const StatusFlash *s, long now) {
    if (!s || now >= s->expires) return 0;
    return s->msg;
}
