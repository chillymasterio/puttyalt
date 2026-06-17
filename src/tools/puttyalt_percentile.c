/* puttyalt_percentile.c - Compute percentiles from samples.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
static void pct_sort(double *a, int n) {
    for (int i = 0; i < n; i++) for (int j = i+1; j < n; j++)
        if (a[j] < a[i]) { double t = a[i]; a[i] = a[j]; a[j] = t; }
}
/* Compute the p-th percentile (0..100) from a copy-sorted buffer. */
double pct_compute(double *samples, int n, double p) {
    if (!samples || n <= 0) return 0;
    pct_sort(samples, n);
    if (p <= 0) return samples[0];
    if (p >= 100) return samples[n-1];
    double rank = p / 100.0 * (n - 1);
    int lo = (int)rank;
    double frac = rank - lo;
    if (lo + 1 < n) return samples[lo] + frac * (samples[lo+1] - samples[lo]);
    return samples[lo];
}
double pct_median(double *samples, int n) { return pct_compute(samples, n, 50.0); }
