/* puttyalt_tabtitle.c - Compute tab titles with truncation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
/* Build a tab title from host + optional index, truncated to width. */
int tt4_build(const char *host, int index, int width, char *out, int outlen) {
    if (!host || !out) return -1;
    char full[160];
    if (index > 0) snprintf(full, sizeof full, "%d: %s", index, host);
    else snprintf(full, sizeof full, "%s", host);
    int n = (int)strlen(full);
    if (n <= width) { strncpy(out, full, outlen-1); out[outlen-1]=0; return (int)strlen(out); }
    int keep = width - 1;
    if (keep < 0) keep = 0;
    if (keep >= outlen) keep = outlen - 2;
    memcpy(out, full, keep);
    out[keep] = '~';
    out[keep+1] = 0;
    return keep + 1;
}
