/* puttyalt_httpauth2.c - Build HTTP Authorization headers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
static const char B64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/* Base64 encode for Basic auth. */
static int ha2_b64(const char *in, char *out, int outlen) {
    int n = (int)strlen(in), o = 0;
    for (int i = 0; i < n; i += 3) {
        int b0 = (unsigned char)in[i];
        int b1 = i+1 < n ? (unsigned char)in[i+1] : 0;
        int b2 = i+2 < n ? (unsigned char)in[i+2] : 0;
        if (o + 4 >= outlen) return -1;
        out[o++] = B64[b0 >> 2];
        out[o++] = B64[((b0 & 3) << 4) | (b1 >> 4)];
        out[o++] = i+1 < n ? B64[((b1 & 15) << 2) | (b2 >> 6)] : '=';
        out[o++] = i+2 < n ? B64[b2 & 63] : '=';
    }
    out[o] = 0;
    return o;
}
int ha2_basic(const char *user, const char *pass, char *out, int outlen) {
    if (!user || !pass || !out) return -1;
    char creds[256];
    snprintf(creds, sizeof creds, "%s:%s", user, pass);
    char b64[384];
    if (ha2_b64(creds, b64, sizeof b64) < 0) return -1;
    return snprintf(out, outlen, "Basic %s", b64);
}
int ha2_bearer(const char *token, char *out, int outlen) {
    if (!token || !out) return -1;
    return snprintf(out, outlen, "Bearer %s", token);
}
