/* puttyalt_sortedset.c - Sorted integer set with binary search.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SS_CAP 256
typedef struct { int data[SS_CAP]; int n; } SortedSet;
void ss4_init(SortedSet *s) { if (s) s->n = 0; }
/* Binary search; returns index if found, else -(insert_pos)-1. */
int ss4_bsearch(const SortedSet *s, int v) {
    if (!s) return -1;
    int lo = 0, hi = s->n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (s->data[mid] == v) return mid;
        if (s->data[mid] < v) lo = mid + 1; else hi = mid - 1;
    }
    return -(lo) - 1;
}
int ss4_insert(SortedSet *s, int v) {
    if (!s || s->n >= SS_CAP) return -1;
    int pos = ss4_bsearch(s, v);
    if (pos >= 0) return 0; /* already present */
    pos = -(pos) - 1;
    for (int i = s->n; i > pos; i--) s->data[i] = s->data[i-1];
    s->data[pos] = v; s->n++;
    return 0;
}
int ss4_contains(const SortedSet *s, int v) { return ss4_bsearch(s, v) >= 0; }
