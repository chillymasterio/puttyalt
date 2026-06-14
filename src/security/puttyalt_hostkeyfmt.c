/* puttyalt_hostkeyfmt.c - Format host key fingerprints.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <stdint.h>
int hkf_md5(const uint8_t *raw, int n, char *out, int outlen) {
    if (!raw || !out || n <= 0) return -1;
    int o = 0;
    for (int i = 0; i < n; i++) {
        if (o + 3 >= outlen) return -1;
        o += snprintf(out + o, outlen - o, "%s%02x", i ? ":" : "", raw[i]);
    }
    return o;
}
/* Bubble-babble style not implemented; provide colon-hex + short id. */
uint32_t hkf_shortid(const uint8_t *raw, int n) {
    uint32_t h = 2166136261u;
    for (int i = 0; i < n; i++) { h ^= raw[i]; h *= 16777619u; }
    return h;
}
