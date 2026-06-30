/* puttyalt_pathjoin.c - Join and split filesystem paths.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
int pj2_join(const char *a, const char *b, char *out, int outlen) {
    if (!a || !b || !out) return -1;
    int al = (int)strlen(a);
    int has_sep = al > 0 && (a[al-1] == '/' || a[al-1] == '\\');
    int skip = (b[0] == '/' || b[0] == '\\') ? 1 : 0;
    int n = snprintf(out, outlen, "%s%s%s", a, has_sep ? "" : "/", b + skip);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int pj2_basename(const char *path, char *out, int outlen) {
    if (!path || !out) return -1;
    const char *slash = strrchr(path, '/');
    const char *bslash = strrchr(path, '\\');
    const char *base = slash > bslash ? slash : bslash;
    base = base ? base + 1 : path;
    strncpy(out, base, outlen-1); out[outlen-1] = 0;
    return (int)strlen(out);
}
int pj2_extension(const char *path, char *out, int outlen) {
    if (!path || !out) return -1;
    const char *dot = strrchr(path, '.');
    const char *slash = strrchr(path, '/');
    if (!dot || (slash && dot < slash)) { out[0] = 0; return 0; }
    strncpy(out, dot + 1, outlen-1); out[outlen-1] = 0;
    return (int)strlen(out);
}
