/* puttyalt_csvparse.c - Parse one CSV record into fields.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Parses a single CSV line into NUL-separated fields in `out`.
   Returns field count, or -1 on overflow. */
int csvp_line(const char *line, char *out, int outlen, int *offsets, int maxf) {
    if (!line || !out || !offsets) return -1;
    int o = 0, f = 0, inq = 0;
    offsets[f] = 0;
    for (const char *p = line; *p; p++) {
        if (inq) {
            if (*p == '"' && p[1] == '"') { if (o >= outlen-1) return -1; out[o++] = '"'; p++; }
            else if (*p == '"') inq = 0;
            else { if (o >= outlen-1) return -1; out[o++] = *p; }
        } else if (*p == '"') inq = 1;
        else if (*p == ',') {
            if (o >= outlen-1) return -1;
            out[o++] = 0;
            if (++f >= maxf) return -1;
            offsets[f] = o;
        } else { if (o >= outlen-1) return -1; out[o++] = *p; }
    }
    out[o] = 0;
    return f + 1;
}
