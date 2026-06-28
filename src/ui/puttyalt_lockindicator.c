/* puttyalt_lockindicator.c - Show capslock/numlock state indicators.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int caps; int num; int scroll; } LockIndicator;
void li2_init(LockIndicator *l) { if (l) { l->caps = 0; l->num = 0; l->scroll = 0; } }
void li2_set(LockIndicator *l, int caps, int num, int scroll) {
    if (!l) return;
    l->caps = caps;
    l->num = num;
    l->scroll = scroll;
}
int li2_format(const LockIndicator *l, char *out, int outlen) {
    if (!l || !out || outlen < 16) return -1;
    int o = 0;
    if (l->caps) { const char *s="CAPS "; while(*s&&o<outlen-1)out[o++]=*s++; }
    if (l->num) { const char *s="NUM "; while(*s&&o<outlen-1)out[o++]=*s++; }
    if (l->scroll) { const char *s="SCRL "; while(*s&&o<outlen-1)out[o++]=*s++; }
    out[o] = 0;
    return o;
}
