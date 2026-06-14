/* puttyalt_portrange.c - Parse port range / list specifications.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
/* Parse "22,80,8000-8010" into a sorted unique set. Returns count. */
int prng_parse(const char *spec, int *out, int outcap) {
    if (!spec || !out) return -1;
    int n = 0;
    const char *p = spec;
    while (*p) {
        int a = 0, b = 0;
        while (*p == ' ' || *p == ',') p++;
        if (sscanf(p, "%d-%d", &a, &b) == 2) {
            while (*p && *p != ',') p++;
        } else if (sscanf(p, "%d", &a) == 1) { b = a; while (*p && *p != ',') p++; }
        else break;
        if (a < 0 || b > 65535 || a > b) return -1;
        for (int v = a; v <= b && n < outcap; v++) out[n++] = v;
    }
    return n;
}
int prng_contains(const int *set, int n, int port) {
    for (int i = 0; i < n; i++) if (set[i] == port) return 1;
    return 0;
}
