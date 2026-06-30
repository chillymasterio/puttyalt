/* puttyalt_matchhighlight.c - Highlight matched substring positions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MH_MAX 32
typedef struct { int positions[MH_MAX]; int n; } MatchHighlight;
/* Find positions of each char of `needle` within `haystack` (fuzzy). */
int mh6_compute(const char *needle, const char *haystack, MatchHighlight *out) {
    if (!needle || !haystack || !out) return -1;
    out->n = 0;
    int hi = 0;
    for (const char *q = needle; *q && out->n < MH_MAX; q++) {
        while (haystack[hi] && haystack[hi] != *q) hi++;
        if (!haystack[hi]) return -1;
        out->positions[out->n++] = hi;
        hi++;
    }
    return out->n;
}
int mh6_is_highlighted(const MatchHighlight *m, int pos) {
    if (!m) return 0;
    for (int i = 0; i < m->n; i++) if (m->positions[i] == pos) return 1;
    return 0;
}
