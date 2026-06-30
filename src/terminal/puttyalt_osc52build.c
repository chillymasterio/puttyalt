/* puttyalt_osc52build.c - Build OSC 52 clipboard sequences.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
static const char O52_B64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/* Build an OSC 52 set-clipboard sequence with base64 of `text`. */
int o52_build(const char *text, char *out, int outlen) {
    if (!text || !out) return -1;
    int o = snprintf(out, outlen, "\x1b]52;c;");
    int n = (int)strlen(text);
    for (int i = 0; i < n; i += 3) {
        int b0 = (unsigned char)text[i];
        int b1 = i+1 < n ? (unsigned char)text[i+1] : 0;
        int b2 = i+2 < n ? (unsigned char)text[i+2] : 0;
        if (o + 4 >= outlen - 2) return -1;
        out[o++] = O52_B64[b0 >> 2];
        out[o++] = O52_B64[((b0 & 3) << 4) | (b1 >> 4)];
        out[o++] = i+1 < n ? O52_B64[((b1 & 15) << 2) | (b2 >> 6)] : '=';
        out[o++] = i+2 < n ? O52_B64[b2 & 63] : '=';
    }
    if (o + 2 >= outlen) return -1;
    out[o++] = '\x07';
    out[o] = 0;
    return o;
}
