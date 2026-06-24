/* puttyalt_caseconv.c - Unicode-aware ASCII case conversion.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int cc4_upper(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s && o < outlen-1; s++) {
        char c = *s;
        out[o++] = (c >= 'a' && c <= 'z') ? c - 32 : c;
    }
    out[o] = 0;
    return o;
}
int cc4_lower(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s && o < outlen-1; s++) {
        char c = *s;
        out[o++] = (c >= 'A' && c <= 'Z') ? c + 32 : c;
    }
    out[o] = 0;
    return o;
}
int cc4_swapcase(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s && o < outlen-1; s++) {
        char c = *s;
        if (c >= 'a' && c <= 'z') out[o++] = c - 32;
        else if (c >= 'A' && c <= 'Z') out[o++] = c + 32;
        else out[o++] = c;
    }
    out[o] = 0;
    return o;
}
