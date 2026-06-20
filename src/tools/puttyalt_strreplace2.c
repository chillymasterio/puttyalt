/* puttyalt_strreplace2.c - Replace all occurrences of a substring.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int srp_all(const char *src, const char *find, const char *repl, char *out, int outlen) {
    if (!src || !find || !repl || !out || !*find) return -1;
    int fl = (int)strlen(find), rl = (int)strlen(repl);
    int o = 0;
    const char *p = src;
    while (*p && o < outlen - 1) {
        if (strncmp(p, find, fl) == 0) {
            if (o + rl >= outlen - 1) return -1;
            memcpy(out + o, repl, rl); o += rl; p += fl;
        } else out[o++] = *p++;
    }
    out[o] = 0;
    return o;
}
int srp_count(const char *src, const char *find) {
    if (!src || !find || !*find) return 0;
    int fl = (int)strlen(find), c = 0;
    for (const char *p = src; (p = strstr(p, find)); p += fl) c++;
    return c;
}
