/* puttyalt_numlocale.c - Format numbers with locale separators.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
/* Group digits with `sep` every 3 places: 1234567 -> "1,234,567". */
int nl2_group(long n, char sep, char *out, int outlen) {
    if (!out) return -1;
    char tmp[32];
    int neg = n < 0;
    unsigned long un = neg ? (unsigned long)(-n) : (unsigned long)n;
    int len = snprintf(tmp, sizeof tmp, "%lu", un);
    int groups = (len - 1) / 3;
    int total = len + groups + (neg ? 1 : 0);
    if (total >= outlen) return -1;
    int o = 0;
    if (neg) out[o++] = '-';
    int first = len % 3; if (first == 0) first = 3;
    int ti = 0;
    for (int g = 0; ti < len; g++) {
        int take = (g == 0) ? first : 3;
        if (g > 0) out[o++] = sep;
        for (int k = 0; k < take; k++) out[o++] = tmp[ti++];
    }
    out[o] = 0;
    return o;
}
