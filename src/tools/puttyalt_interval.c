/* puttyalt_interval.c - Interval overlap / merge helpers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long lo, hi; } Interval;
int iv_overlap(Interval a, Interval b) { return a.lo <= b.hi && b.lo <= a.hi; }
int iv_contains(Interval a, long p) { return p >= a.lo && p <= a.hi; }
Interval iv_union(Interval a, Interval b) {
    Interval r; r.lo = a.lo < b.lo ? a.lo : b.lo; r.hi = a.hi > b.hi ? a.hi : b.hi; return r;
}
/* In-place merge of sorted intervals; returns new count. */
int iv_merge(Interval *iv, int n) {
    if (n <= 1) return n;
    int w = 0;
    for (int i = 1; i < n; i++) {
        if (iv[i].lo <= iv[w].hi) { if (iv[i].hi > iv[w].hi) iv[w].hi = iv[i].hi; }
        else iv[++w] = iv[i];
    }
    return w + 1;
}
