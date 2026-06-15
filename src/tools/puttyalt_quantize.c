/* puttyalt_quantize.c - Bucket values into histogram bins.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define QZ_BINS 32
typedef struct { long count[QZ_BINS]; double lo, hi; int nbins; } Quantizer;
void qz_init(Quantizer *q, double lo, double hi, int nbins) {
    if (!q) return;
    memset(q->count, 0, sizeof q->count);
    q->lo = lo; q->hi = hi > lo ? hi : lo + 1;
    q->nbins = (nbins > 0 && nbins <= QZ_BINS) ? nbins : QZ_BINS;
}
int qz_bin(const Quantizer *q, double v) {
    if (!q) return -1;
    if (v <= q->lo) return 0;
    if (v >= q->hi) return q->nbins - 1;
    int b = (int)((v - q->lo) / (q->hi - q->lo) * q->nbins);
    if (b >= q->nbins) b = q->nbins - 1;
    return b;
}
void qz_add(Quantizer *q, double v) { int b = qz_bin(q, v); if (b >= 0) q->count[b]++; }
long qz_peak(const Quantizer *q) {
    if (!q) return 0;
    long m = 0;
    for (int i = 0; i < q->nbins; i++) if (q->count[i] > m) m = q->count[i];
    return m;
}
