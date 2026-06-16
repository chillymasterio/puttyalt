/* puttyalt_pathnorm.c - Normalize filesystem paths.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Collapse "." and ".." and duplicate slashes in a POSIX-style path. */
int pn_normalize(const char *path, char *out, int outlen) {
    if (!path || !out) return -1;
    char parts[64][64]; int np = 0;
    int absolute = (path[0] == '/');
    const char *p = path;
    while (*p) {
        while (*p == '/') p++;
        if (!*p) break;
        char seg[64]; int sl = 0;
        while (*p && *p != '/' && sl < 63) seg[sl++] = *p++;
        seg[sl] = 0;
        if (strcmp(seg, ".") == 0) continue;
        if (strcmp(seg, "..") == 0) { if (np > 0 && strcmp(parts[np-1],"..")!=0) np--; else if (!absolute) { strcpy(parts[np++], ".."); } continue; }
        if (np < 64) strcpy(parts[np++], seg);
    }
    int o = 0;
    if (absolute && o < outlen-1) out[o++] = '/';
    for (int i = 0; i < np; i++) {
        if (i && o < outlen-1) out[o++] = '/';
        for (const char *q = parts[i]; *q && o < outlen-1; q++) out[o++] = *q;
    }
    if (o == 0 && o < outlen-1) out[o++] = '.';
    out[o] = 0;
    return o;
}
