/* puttyalt_hexcodec.c - Hex string encode/decode.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int hc2_encode(const unsigned char *in, int n, char *out, int outlen) {
    static const char H[] = "0123456789abcdef";
    if (!in || !out || outlen < n*2+1) return -1;
    int o = 0;
    for (int i = 0; i < n; i++) { out[o++] = H[in[i]>>4]; out[o++] = H[in[i]&15]; }
    out[o] = 0;
    return o;
}
static int hc2_nib(char c) {
    if (c>='0'&&c<='9') return c-'0';
    if (c>='a'&&c<='f') return c-'a'+10;
    if (c>='A'&&c<='F') return c-'A'+10;
    return -1;
}
int hc2_decode(const char *in, unsigned char *out, int outcap) {
    if (!in || !out) return -1;
    int o = 0;
    while (in[0] && in[1]) {
        int h = hc2_nib(in[0]), l = hc2_nib(in[1]);
        if (h < 0 || l < 0) return -1;
        if (o >= outcap) return -1;
        out[o++] = (unsigned char)((h<<4)|l);
        in += 2;
    }
    return o;
}
