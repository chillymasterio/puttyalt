/* puttyalt_dnsname.c - Encode/decode DNS names (label format).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdint.h>
/* Encode "www.example.com" into DNS label format. Returns length. */
int dn3_encode(const char *name, uint8_t *out, int cap) {
    if (!name || !out) return -1;
    int o = 0;
    const char *p = name;
    while (*p) {
        const char *dot = strchr(p, '.');
        int len = dot ? (int)(dot - p) : (int)strlen(p);
        if (len > 63 || o + len + 1 >= cap) return -1;
        out[o++] = (uint8_t)len;
        memcpy(out + o, p, len); o += len;
        if (!dot) break;
        p = dot + 1;
    }
    if (o >= cap) return -1;
    out[o++] = 0;
    return o;
}
int dn3_decode(const uint8_t *in, int n, char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0, i = 0;
    while (i < n && in[i]) {
        int len = in[i++];
        if (len > 63 || i + len > n) return -1;
        if (o && o < outlen-1) out[o++] = '.';
        for (int k = 0; k < len && o < outlen-1; k++) out[o++] = (char)in[i++];
    }
    out[o] = 0;
    return o;
}
