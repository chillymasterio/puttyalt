/* puttyalt_progressbar.c - Render a text progress bar.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
int pb_render(int cur, int total, int width, char *out, int outlen) {
    if (!out || width < 4 || outlen < width + 8) return -1;
    if (total <= 0) total = 1;
    if (cur < 0) cur = 0;
    if (cur > total) cur = total;
    int inner = width - 2;
    int filled = (int)((long)cur * inner / total);
    int pct = (int)((long)cur * 100 / total);
    int o = 0;
    out[o++] = '[';
    for (int i = 0; i < inner; i++) out[o++] = (i < filled) ? '#' : '-';
    out[o++] = ']';
    o += snprintf(out + o, outlen - o, " %3d%%", pct);
    out[o] = 0;
    return o;
}
