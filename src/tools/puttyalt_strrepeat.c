/* puttyalt_strrepeat.c - Repeat and reverse strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int sr5_repeat(const char *s, int times, char *out, int outlen) {
    if (!s || !out || times < 0) return -1;
    int sl = (int)strlen(s), o = 0;
    for (int i = 0; i < times; i++) {
        if (o + sl >= outlen - 1) return -1;
        memcpy(out + o, s, sl); o += sl;
    }
    out[o] = 0;
    return o;
}
int sr5_reverse(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int n = (int)strlen(s);
    if (n >= outlen) return -1;
    for (int i = 0; i < n; i++) out[i] = s[n-1-i];
    out[n] = 0;
    return n;
}
char sr5_char_repeat(char c, int times, char *out, int outlen) {
    int o = 0;
    for (; o < times && o < outlen - 1; o++) out[o] = c;
    out[o] = 0;
    return c;
}
