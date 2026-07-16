/* puttyalt_pastefilter2.c - Filter bracketed-paste control content.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Remove embedded paste-end markers to prevent paste injection. */
int pfl_sanitize(const char *in, int n, char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0;
    for (int i = 0; i < n && o < outlen - 1; i++) {
        if (in[i] == 0x1b && i + 5 < n && !strncmp(in + i, "\x1b[201~", 6)) { i += 5; continue; }
        out[o++] = in[i];
    }
    out[o] = 0;
    return o;
}
int pfl_wrap(const char *text, char *out, int outlen) {
    if (!text || !out) return -1;
    int w = 0;
    for (const char *s = "\x1b[200~"; *s && w < outlen - 1; s++) out[w++] = *s;
    for (; *text && w < outlen - 7; text++) out[w++] = *text;
    for (const char *e = "\x1b[201~"; *e && w < outlen - 1; e++) out[w++] = *e;
    out[w] = 0;
    return w;
}
