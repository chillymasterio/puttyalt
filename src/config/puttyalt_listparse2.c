/* puttyalt_listparse2.c - Parse comma/space separated lists.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
/* Parse a delimited list into NUL-separated tokens; returns count. */
int lp4_parse(const char *s, char delim, char *out, int outlen, int *offsets, int maxtok) {
    if (!s || !out || !offsets) return -1;
    int o = 0, n = 0;
    const char *p = s;
    while (*p && n < maxtok) {
        while (*p == delim || isspace((unsigned char)*p)) p++;
        if (!*p) break;
        offsets[n++] = o;
        while (*p && *p != delim && o < outlen - 1) {
            if (isspace((unsigned char)*p) && (p[1] == delim || p[1] == 0)) { p++; continue; }
            out[o++] = *p++;
        }
        if (o < outlen - 1) out[o++] = 0;
        while (*p && *p != delim) p++;
    }
    return n;
}
