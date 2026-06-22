/* puttyalt_prettybytes.c - Format byte sizes human-readable.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int pby_format(long bytes, char *out, int outlen) {
    if (!out) return -1;
    const char *units[] = {"B","KB","MB","GB","TB","PB"};
    double v = (double)bytes;
    int u = 0;
    while (v >= 1024.0 && u < 5) { v /= 1024.0; u++; }
    if (u == 0) return snprintf(out, outlen, "%ld %s", bytes, units[0]);
    return snprintf(out, outlen, "%.1f %s", v, units[u]);
}
int pby_format_si(long n, char *out, int outlen) {
    if (!out) return -1;
    const char *units[] = {"","K","M","G","T"};
    double v = (double)n;
    int u = 0;
    while (v >= 1000.0 && u < 4) { v /= 1000.0; u++; }
    if (u == 0) return snprintf(out, outlen, "%ld", n);
    return snprintf(out, outlen, "%.1f%s", v, units[u]);
}
