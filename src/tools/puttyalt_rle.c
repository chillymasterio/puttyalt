/* puttyalt_rle.c - Run-length encode/decode byte runs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int rle_encode(const unsigned char *in, int n, unsigned char *out, int outcap) {
    if (!in || !out || n < 0) return -1;
    int o = 0, i = 0;
    while (i < n) {
        unsigned char v = in[i]; int run = 1;
        while (i + run < n && in[i+run] == v && run < 255) run++;
        if (o + 2 > outcap) return -1;
        out[o++] = (unsigned char)run; out[o++] = v;
        i += run;
    }
    return o;
}
int rle_decode(const unsigned char *in, int n, unsigned char *out, int outcap) {
    if (!in || !out || (n & 1)) return -1;
    int o = 0;
    for (int i = 0; i + 1 < n; i += 2) {
        int run = in[i];
        if (o + run > outcap) return -1;
        for (int k = 0; k < run; k++) out[o++] = in[i+1];
    }
    return o;
}
