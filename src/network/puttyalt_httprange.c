/* puttyalt_httprange.c - Build/parse HTTP Range headers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int hr_build(long start, long end, char *out, int outlen) {
    if (!out) return -1;
    if (end < 0) return snprintf(out, outlen, "bytes=%ld-", start);
    return snprintf(out, outlen, "bytes=%ld-%ld", start, end);
}
int hr_parse(const char *hdr, long *start, long *end) {
    if (!hdr || !start || !end) return -1;
    long s = 0, e = -1;
    if (sscanf(hdr, "bytes=%ld-%ld", &s, &e) >= 1) { *start = s; *end = e; return 0; }
    return -1;
}
/* Parse "Content-Range: bytes 0-99/200" total size. */
long hr_total(const char *hdr) {
    const char *slash = hdr ? __builtin_strchr(hdr, '/') : 0;
    if (!slash) return -1;
    long t = -1;
    if (sscanf(slash + 1, "%ld", &t) == 1) return t;
    return -1;
}
