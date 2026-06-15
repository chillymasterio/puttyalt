/* puttyalt_minheap.c - Binary min-heap of ints.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define MH_CAP 256
typedef struct { int a[MH_CAP]; int n; } MinHeap;
void mh_init(MinHeap *h) { if (h) h->n = 0; }
int mh_push(MinHeap *h, int v) {
    if (!h || h->n >= MH_CAP) return -1;
    int i = h->n++; h->a[i] = v;
    while (i > 0) { int p = (i-1)/2; if (h->a[p] <= h->a[i]) break; int t=h->a[p]; h->a[p]=h->a[i]; h->a[i]=t; i=p; }
    return 0;
}
int mh_pop(MinHeap *h, int *out) {
    if (!h || h->n == 0) return -1;
    if (out) *out = h->a[0];
    h->a[0] = h->a[--h->n];
    int i = 0;
    for (;;) {
        int l = 2*i+1, r = 2*i+2, m = i;
        if (l < h->n && h->a[l] < h->a[m]) m = l;
        if (r < h->n && h->a[r] < h->a[m]) m = r;
        if (m == i) break;
        int t=h->a[m]; h->a[m]=h->a[i]; h->a[i]=t; i=m;
    }
    return 0;
}
int mh_peek(const MinHeap *h, int *out) { if (!h || h->n==0) return -1; if (out) *out=h->a[0]; return 0; }
