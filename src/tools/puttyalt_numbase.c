/* puttyalt_numbase.c - Convert integers between bases 2-36.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int nb_to_string(long v, int base, char *out, int outlen) {
    if (!out || base < 2 || base > 36 || outlen < 2) return -1;
    static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char tmp[72]; int ti = 0;
    int neg = v < 0;
    unsigned long uv = neg ? (unsigned long)(-v) : (unsigned long)v;
    if (uv == 0) tmp[ti++] = '0';
    while (uv) { tmp[ti++] = digits[uv % base]; uv /= base; }
    int o = 0;
    if (neg && o < outlen - 1) out[o++] = '-';
    while (ti > 0 && o < outlen - 1) out[o++] = tmp[--ti];
    out[o] = 0;
    return o;
}
long nb_from_string(const char *s, int base) {
    if (!s || base < 2 || base > 36) return 0;
    long v = 0; int neg = 0;
    if (*s == '-') { neg = 1; s++; }
    for (; *s; s++) {
        int d;
        if (*s >= '0' && *s <= '9') d = *s - '0';
        else if (*s >= 'a' && *s <= 'z') d = *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'Z') d = *s - 'A' + 10;
        else break;
        if (d >= base) break;
        v = v * base + d;
    }
    return neg ? -v : v;
}
