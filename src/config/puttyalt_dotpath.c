/* puttyalt_dotpath.c - Navigate nested config by dotted path.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Split "a.b.c" into segments. Returns count. */
int dp2_split(const char *path, char seg[][48], int maxseg) {
    if (!path) return 0;
    int n = 0;
    const char *p = path;
    while (*p && n < maxseg) {
        int o = 0;
        while (*p && *p != '.' && o < 47) seg[n][o++] = *p++;
        seg[n][o] = 0;
        n++;
        if (*p == '.') p++;
    }
    return n;
}
int dp2_depth(const char *path) {
    if (!path || !*path) return 0;
    int d = 1;
    for (const char *p = path; *p; p++) if (*p == '.') d++;
    return d;
}
int dp2_parent(const char *path, char *out, int outlen) {
    if (!path || !out) return -1;
    const char *dot = strrchr(path, '.');
    if (!dot) { out[0] = 0; return 0; }
    int len = (int)(dot - path);
    if (len >= outlen) len = outlen - 1;
    memcpy(out, path, len); out[len] = 0;
    return len;
}
