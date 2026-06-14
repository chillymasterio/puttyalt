/* puttyalt_base32hex.c - Base32 (RFC 4648) encode/decode.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static const char B32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
int b32h_encode(const unsigned char *in, int n, char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0; unsigned long buf = 0; int bits = 0;
    for (int i = 0; i < n; i++) {
        buf = (buf << 8) | in[i]; bits += 8;
        while (bits >= 5) { if (o >= outlen-1) return -1; out[o++] = B32[(buf >> (bits-5)) & 31]; bits -= 5; }
    }
    if (bits > 0) { if (o >= outlen-1) return -1; out[o++] = B32[(buf << (5-bits)) & 31]; }
    out[o] = 0;
    return o;
}
static int b32h_val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= '2' && c <= '7') return c - '2' + 26;
    return -1;
}
int b32h_decode(const char *in, unsigned char *out, int outcap) {
    if (!in || !out) return -1;
    int o = 0; unsigned long buf = 0; int bits = 0;
    for (; *in && *in != '='; in++) {
        int v = b32h_val(*in); if (v < 0) return -1;
        buf = (buf << 5) | v; bits += 5;
        if (bits >= 8) { if (o >= outcap) return -1; out[o++] = (unsigned char)((buf >> (bits-8)) & 0xFF); bits -= 8; }
    }
    return o;
}
