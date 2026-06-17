/* puttyalt_flexbox.c - Distribute space among flex items.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Distribute `total` pixels among items by weight; writes sizes. */
int flx_distribute(const int *weights, int n, int total, int *out) {
    if (!weights || !out || n <= 0) return -1;
    long sum = 0;
    for (int i = 0; i < n; i++) sum += weights[i] > 0 ? weights[i] : 0;
    if (sum == 0) { for (int i = 0; i < n; i++) out[i] = total / n; return 0; }
    int assigned = 0;
    for (int i = 0; i < n; i++) {
        out[i] = (int)((long)total * (weights[i] > 0 ? weights[i] : 0) / sum);
        assigned += out[i];
    }
    /* give rounding remainder to the last flexible item */
    for (int i = n - 1; i >= 0 && assigned < total; i--)
        if (weights[i] > 0) { out[i] += total - assigned; break; }
    return 0;
}
