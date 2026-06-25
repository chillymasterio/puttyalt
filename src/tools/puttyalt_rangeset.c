/* puttyalt_rangeset.c - Manage sets of integer ranges.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define RS_CAP 32
typedef struct { long lo, hi; } RsRange;
typedef struct { RsRange r[RS_CAP]; int n; } RangeSet;
void rs4_init(RangeSet *s) { if (s) s->n = 0; }
int rs4_add(RangeSet *s, long lo, long hi) {
    if (!s || s->n >= RS_CAP || lo > hi) return -1;
    s->r[s->n].lo = lo; s->r[s->n].hi = hi; s->n++;
    return 0;
}
int rs4_contains(const RangeSet *s, long v) {
    if (!s) return 0;
    for (int i = 0; i < s->n; i++) if (v >= s->r[i].lo && v <= s->r[i].hi) return 1;
    return 0;
}
long rs4_total(const RangeSet *s) {
    if (!s) return 0;
    long t = 0;
    for (int i = 0; i < s->n; i++) t += s->r[i].hi - s->r[i].lo + 1;
    return t;
}
