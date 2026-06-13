/* puttyalt_base64url.c - Base64url encode/decode (RFC 4648 URL-safe). */
#include <string.h>
static const char B64U[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
int base64url_encode(const unsigned char *in, int len, char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0;
    for (int i = 0; i < len; i += 3) {
        unsigned int v = in[i] << 16;
        if (i + 1 < len) v |= in[i + 1] << 8;
        if (i + 2 < len) v |= in[i + 2];
        if (o < outlen - 1) out[o++] = B64U[(v >> 18) & 0x3F];
        if (o < outlen - 1) out[o++] = B64U[(v >> 12) & 0x3F];
        if (i + 1 < len && o < outlen - 1) out[o++] = B64U[(v >> 6) & 0x3F];
        if (i + 2 < len && o < outlen - 1) out[o++] = B64U[v & 0x3F];
    }
    out[o] = 0;
    return o;
}
static int b64u_val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '-') return 62;
    if (c == '_') return 63;
    return -1;
}
int base64url_decode(const char *in, unsigned char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0, bits = 0;
    unsigned long buf = 0;
    for (const char *p = in; *p; p++) {
        int v = b64u_val(*p);
        if (v < 0) continue;
        buf = (buf << 6) | v;
        bits += 6;
        if (bits >= 8 && o < outlen) {
            out[o++] = (buf >> (bits - 8)) & 0xFF;
            bits -= 8;
        }
    }
    return o;
}
