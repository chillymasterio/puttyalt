/* puttyalt_urlencode.c - Percent-encode/decode URL components.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
static int ule_hex(int c) { return c < 10 ? '0' + c : 'A' + (c - 10); }
int ule_encode(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s; s++) {
        unsigned char c = (unsigned char)*s;
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            if (o + 1 >= outlen) return -1;
            out[o++] = (char)c;
        } else {
            if (o + 3 >= outlen) return -1;
            out[o++] = '%'; out[o++] = (char)ule_hex(c >> 4); out[o++] = (char)ule_hex(c & 15);
        }
    }
    out[o] = 0;
    return o;
}
static int ule_unhex(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
int ule_decode(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s; s++) {
        if (o + 1 >= outlen) return -1;
        if (*s == '%' && s[1] && s[2]) {
            int h = ule_unhex(s[1]), l = ule_unhex(s[2]);
            if (h < 0 || l < 0) return -1;
            out[o++] = (char)((h << 4) | l); s += 2;
        } else if (*s == '+') out[o++] = ' ';
        else out[o++] = *s;
    }
    out[o] = 0;
    return o;
}
