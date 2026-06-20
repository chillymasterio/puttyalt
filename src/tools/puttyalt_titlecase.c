/* puttyalt_titlecase.c - Convert strings to Title/camel/snake case.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
int tc3_title(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0, start = 1;
    for (; *s && o < outlen-1; s++) {
        if (isspace((unsigned char)*s)) { out[o++] = *s; start = 1; }
        else { out[o++] = start ? (char)toupper((unsigned char)*s) : (char)tolower((unsigned char)*s); start = 0; }
    }
    out[o] = 0;
    return o;
}
int tc3_snake(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s && o < outlen-1; s++) {
        if (isupper((unsigned char)*s)) {
            if (o > 0 && o < outlen-2) out[o++] = '_';
            out[o++] = (char)tolower((unsigned char)*s);
        } else if (isspace((unsigned char)*s)) out[o++] = '_';
        else out[o++] = *s;
    }
    out[o] = 0;
    return o;
}
