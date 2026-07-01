/* puttyalt_basexx.c - Convert integers to arbitrary base strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int bxx_encode(unsigned long v, int base, const char *alphabet, char *out, int outlen) {
    if (!alphabet || !out || base < 2 || base > 62) return -1;
    char tmp[72]; int ti = 0;
    if (v == 0) tmp[ti++] = alphabet[0];
    while (v) { tmp[ti++] = alphabet[v % base]; v /= base; }
    int o = 0;
    while (ti > 0 && o < outlen - 1) out[o++] = tmp[--ti];
    out[o] = 0;
    return o;
}
long bxx_decode(const char *s, int base, const char *alphabet) {
    if (!s || !alphabet) return -1;
    long v = 0;
    for (; *s; s++) {
        const char *pos = strchr(alphabet, *s);
        if (!pos) break;
        int d = (int)(pos - alphabet);
        if (d >= base) break;
        v = v * base + d;
    }
    return v;
}
