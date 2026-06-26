/* puttyalt_escfilter.c - Filter unsafe escape sequences from output.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Strip dangerous escape sequences (OSC that set clipboard, etc).
   Copies safe content into `out`. Returns output length. */
int ef2_filter(const char *in, int n, char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0, i = 0;
    while (i < n && o < outlen - 1) {
        if (in[i] == 0x1b && i+1 < n && in[i+1] == ']') {
            /* OSC: skip until BEL or ST */
            i += 2;
            while (i < n && in[i] != 0x07 && !(in[i] == 0x1b && i+1 < n && in[i+1] == '\\')) i++;
            if (i < n && in[i] == 0x07) i++;
            else if (i+1 < n) i += 2;
            continue;
        }
        out[o++] = in[i++];
    }
    out[o] = 0;
    return o;
}
/* Count escape sequences in a buffer. */
int ef2_count_esc(const char *in, int n) {
    int c = 0;
    for (int i = 0; i < n; i++) if (in[i] == 0x1b) c++;
    return c;
}
