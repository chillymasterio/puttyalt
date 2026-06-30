/* puttyalt_histogram2.c - Fixed-bucket histogram with percentiles.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define HG_BUCKETS 20
typedef struct { long counts[HG_BUCKETS]; long total; double lo, hi; } Histogram2;
void hg2_init(Histogram2 *h, double lo, double hi) {
    if (!h) return;
    for (int i = 0; i < HG_BUCKETS; i++) h->counts[i] = 0;
    h->total = 0; h->lo = lo; h->hi = hi > lo ? hi : lo + 1;
}
void hg2_add(Histogram2 *h, double v) {
    if (!h) return;
    int b = (int)((v - h->lo) / (h->hi - h->lo) * HG_BUCKETS);
    if (b < 0) b = 0;
    if (b >= HG_BUCKETS) b = HG_BUCKETS - 1;
    h->counts[b]++; h->total++;
}
/* Approximate percentile value (0..100). */
double hg2_percentile(const Histogram2 *h, int pct) {
    if (!h || h->total == 0) return h ? h->lo : 0;
    long target = h->total * pct / 100;
    long acc = 0;
    for (int i = 0; i < HG_BUCKETS; i++) {
        acc += h->counts[i];
        if (acc >= target)
            return h->lo + (h->hi - h->lo) * (i + 0.5) / HG_BUCKETS;
    }
    return h->hi;
}
