/* puttyalt_strsplit2.c - Split a string on a delimiter.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Split `s` on `delim` char into NUL-separated fields; return field count. */
int spl_char(const char *s, char delim, char *out, int outlen, int *offsets, int maxf) {
    if (!s || !out || !offsets) return -1;
    int o = 0, f = 0;
    offsets[0] = 0;
    for (const char *p = s; ; p++) {
        if (*p == delim || *p == 0) {
            if (o < outlen - 1) out[o++] = 0;
            if (*p == 0) { return f + 1; }
            if (++f >= maxf) return -1;
            offsets[f] = o;
        } else if (o < outlen - 1) out[o++] = *p;
    }
}
/* Count fields without writing. */
int spl_count(const char *s, char delim) {
    if (!s) return 0;
    int n = 1;
    for (const char *p = s; *p; p++) if (*p == delim) n++;
    return n;
}
