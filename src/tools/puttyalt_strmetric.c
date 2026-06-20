/* puttyalt_strmetric.c - String distance metrics (Jaro, Hamming).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int sm4_hamming(const char *a, const char *b) {
    if (!a || !b) return -1;
    if (strlen(a) != strlen(b)) return -1;
    int d = 0;
    for (int i = 0; a[i]; i++) if (a[i] != b[i]) d++;
    return d;
}
/* Jaro similarity * 1000 (integer). */
int sm4_jaro_x1000(const char *a, const char *b) {
    if (!a || !b) return 0;
    int la = (int)strlen(a), lb = (int)strlen(b);
    if (la == 0 && lb == 0) return 1000;
    if (la == 0 || lb == 0) return 0;
    int md = (la > lb ? la : lb) / 2 - 1;
    if (md < 0) md = 0;
    char am[256] = {0}, bm[256] = {0};
    if (la > 255 || lb > 255) return 0;
    int matches = 0;
    for (int i = 0; i < la; i++) {
        int lo = i - md > 0 ? i - md : 0;
        int hi = i + md + 1 < lb ? i + md + 1 : lb;
        for (int j = lo; j < hi; j++)
            if (!bm[j] && a[i] == b[j]) { am[i] = bm[j] = 1; matches++; break; }
    }
    if (matches == 0) return 0;
    int t = 0, k = 0;
    for (int i = 0; i < la; i++)
        if (am[i]) { while (!bm[k]) k++; if (a[i] != b[k]) t++; k++; }
    t /= 2;
    double m = matches;
    double jaro = (m/la + m/lb + (m-t)/m) / 3.0;
    return (int)(jaro * 1000);
}
