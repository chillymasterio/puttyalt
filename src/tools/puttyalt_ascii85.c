/* puttyalt_ascii85.c - Ascii85 encode/decode.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int a85_encode(const unsigned char *in, int n, char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0;
    for (int i = 0; i < n; i += 4) {
        unsigned long v = 0; int cnt = 0;
        for (int k = 0; k < 4; k++) { v = (v << 8) | (i + k < n ? in[i+k] : 0); if (i + k < n) cnt++; }
        char grp[5];
        for (int k = 4; k >= 0; k--) { grp[k] = (char)('!' + v % 85); v /= 85; }
        int emit = cnt + 1;
        for (int k = 0; k < emit; k++) { if (o >= outlen - 1) return -1; out[o++] = grp[k]; }
    }
    out[o] = 0;
    return o;
}
int a85_decode(const char *in, unsigned char *out, int outcap) {
    if (!in || !out) return -1;
    int o = 0; unsigned long v = 0; int cnt = 0;
    for (; *in; in++) {
        if (*in < '!' || *in > 'u') continue;
        v = v * 85 + (*in - '!'); cnt++;
        if (cnt == 5) {
            for (int k = 3; k >= 0; k--) { if (o >= outcap) return -1; out[o++] = (unsigned char)(v >> (k*8)); }
            v = 0; cnt = 0;
        }
    }
    if (cnt > 0) {
        for (int k = cnt; k < 5; k++) v = v * 85 + 84;
        for (int k = 0; k < cnt - 1; k++) { if (o >= outcap) return -1; out[o++] = (unsigned char)(v >> ((3-k)*8)); }
    }
    return o;
}
