/* puttyalt_abbreviate.c - Abbreviate long paths and names.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Abbreviate a path like /a/b/c/d to /a/.../d within maxlen. */
int abbr_path(const char *path, int maxlen, char *out, int outlen) {
    if (!path || !out) return -1;
    int n = (int)strlen(path);
    if (n <= maxlen) {
        strncpy(out, path, outlen - 1);
        out[outlen - 1] = 0;
        return (int)strlen(out);
    }
    const char *last = strrchr(path, '/');
    if (!last) last = path;
    const char *first = strchr(path[0] == '/' ? path + 1 : path, '/');
    int hl = (first && first < last) ? (int)(first - path) : 0;
    int o = 0;
    for (int i = 0; i < hl && o < outlen - 1; i++) out[o++] = path[i];
    for (const char *m = "/..."; *m && o < outlen - 1; m++) out[o++] = *m;
    for (const char *q = last; *q && o < outlen - 1; q++) out[o++] = *q;
    out[o] = 0;
    return o;
}
