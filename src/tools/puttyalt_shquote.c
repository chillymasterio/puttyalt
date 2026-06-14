/* puttyalt_shquote.c - Quote/escape arguments for a POSIX shell.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
int shq_quote(const char *s, char *out, int outlen) {
    if (!s || !out || outlen < 3) return -1;
    int safe = 1;
    for (const char *p = s; *p; p++) {
        char c = *p;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '.' || c == '/')) {
            safe = 0; break;
        }
    }
    if (safe) { int n = (int)strlen(s); if (n >= outlen) return -1; memcpy(out, s, n); out[n] = 0; return n; }
    int o = 0;
    out[o++] = '\'';
    for (const char *p = s; *p && o < outlen - 2; p++) {
        if (*p == '\'') {
            if (o + 4 >= outlen) return -1;
            out[o++] = '\''; out[o++] = '\\'; out[o++] = '\''; out[o++] = '\'';
        } else out[o++] = *p;
    }
    if (o >= outlen - 1) return -1;
    out[o++] = '\''; out[o] = 0;
    return o;
}
