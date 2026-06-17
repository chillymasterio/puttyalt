/* puttyalt_badge.c - Format notification count badges.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int bdg_format(int count, char *out, int outlen) {
    if (!out) return -1;
    if (count <= 0) { out[0] = 0; return 0; }
    if (count > 99) return snprintf(out, outlen, "99+");
    return snprintf(out, outlen, "%d", count);
}
int bdg_width(int count) {
    if (count <= 0) return 0;
    if (count > 99) return 3;
    if (count > 9) return 2;
    return 1;
}
