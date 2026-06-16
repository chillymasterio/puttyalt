/* puttyalt_ansilen.c - Measure visible length ignoring ANSI codes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Count display chars, skipping ESC[...m and similar CSI sequences. */
int al_visible_len(const char *s) {
    if (!s) return 0;
    int len = 0;
    while (*s) {
        if (*s == 0x1b && s[1] == '[') {
            s += 2;
            while (*s && !(*s >= 0x40 && *s <= 0x7e)) s++;
            if (*s) s++;
        } else { len++; s++; }
    }
    return len;
}
/* Strip ANSI sequences into `out`. Returns stripped length. */
int al_strip(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    while (*s && o < outlen - 1) {
        if (*s == 0x1b && s[1] == '[') {
            s += 2;
            while (*s && !(*s >= 0x40 && *s <= 0x7e)) s++;
            if (*s) s++;
        } else out[o++] = *s++;
    }
    out[o] = 0;
    return o;
}
