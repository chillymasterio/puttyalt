/* puttyalt_textalign.c - Align text within a fixed width.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int ta3_left(const char *s, int width, char *out, int outlen) {
    if (!s || !out || width >= outlen) return -1;
    int len = (int)strlen(s), o = 0;
    for (int i = 0; i < len && o < width; i++) out[o++] = s[i];
    while (o < width) out[o++] = ' ';
    out[o] = 0;
    return o;
}
int ta3_right(const char *s, int width, char *out, int outlen) {
    if (!s || !out || width >= outlen) return -1;
    int len = (int)strlen(s), o = 0;
    int pad = width - len; if (pad < 0) pad = 0;
    while (o < pad) out[o++] = ' ';
    for (int i = 0; i < len && o < width; i++) out[o++] = s[i];
    out[o] = 0;
    return o;
}
int ta3_center(const char *s, int width, char *out, int outlen) {
    if (!s || !out || width >= outlen) return -1;
    int len = (int)strlen(s);
    int total_pad = width - len; if (total_pad < 0) total_pad = 0;
    int left = total_pad / 2, o = 0;
    while (o < left) out[o++] = ' ';
    for (int i = 0; i < len && o < width; i++) out[o++] = s[i];
    while (o < width) out[o++] = ' ';
    out[o] = 0;
    return o;
}
