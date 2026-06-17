/* puttyalt_sparkline2.c - Render unicode sparkline from values.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Map a value within [min,max] to a sparkline tier 0-7. */
int sk2_tier(double v, double min, double max) {
    if (max <= min) return 0;
    int t = (int)((v - min) / (max - min) * 7.0 + 0.5);
    if (t < 0) t = 0;
    if (t > 7) t = 7;
    return t;
}
/* Fill `tiers` (0-7) for `n` values; computes min/max internally. */
int sk2_build(const double *vals, int n, int *tiers) {
    if (!vals || !tiers || n <= 0) return -1;
    double mn = vals[0], mx = vals[0];
    for (int i = 1; i < n; i++) { if (vals[i] < mn) mn = vals[i]; if (vals[i] > mx) mx = vals[i]; }
    for (int i = 0; i < n; i++) tiers[i] = sk2_tier(vals[i], mn, mx);
    return 0;
}
