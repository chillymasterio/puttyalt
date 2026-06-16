/* puttyalt_jwtdecode.c - Split and inspect JWT token structure.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { char header[256]; char payload[512]; int valid_format; } JwtParts;
/* Split a JWT into its three dot-separated base64url segments (not decoded). */
int jwt_split(const char *token, JwtParts *out) {
    if (!token || !out) return -1;
    out->valid_format = 0;
    const char *d1 = strchr(token, '.');
    if (!d1) return -1;
    const char *d2 = strchr(d1 + 1, '.');
    if (!d2) return -1;
    int hlen = (int)(d1 - token);
    int plen = (int)(d2 - (d1 + 1));
    if (hlen >= 256 || plen >= 512) return -1;
    memcpy(out->header, token, hlen); out->header[hlen] = 0;
    memcpy(out->payload, d1 + 1, plen); out->payload[plen] = 0;
    out->valid_format = 1;
    return 0;
}
int jwt_count_segments(const char *token) {
    if (!token) return 0;
    int n = 1;
    for (const char *p = token; *p; p++) if (*p == '.') n++;
    return n;
}
