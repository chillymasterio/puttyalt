/* puttyalt_textindent.c - Indent and dedent text blocks.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int ti2_indent(const char *s, const char *prefix, char *out, int outlen) {
    if (!s || !prefix || !out) return -1;
    int o = 0, at_line_start = 1;
    int pl = (int)strlen(prefix);
    for (; *s && o < outlen - 1; s++) {
        if (at_line_start && *s != '\n') {
            if (o + pl >= outlen - 1) return -1;
            memcpy(out + o, prefix, pl); o += pl;
            at_line_start = 0;
        }
        out[o++] = *s;
        if (*s == '\n') at_line_start = 1;
    }
    out[o] = 0;
    return o;
}
/* Remove up to `n` leading spaces from each line. */
int ti2_dedent(const char *s, int n, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0, at_start = 1, removed = 0;
    for (; *s && o < outlen - 1; s++) {
        if (at_start && *s == ' ' && removed < n) { removed++; continue; }
        if (*s == '\n') { at_start = 1; removed = 0; }
        else at_start = 0;
        out[o++] = *s;
    }
    out[o] = 0;
    return o;
}
