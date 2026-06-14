/* puttyalt_macaddr.c - Parse and format MAC addresses.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <stdint.h>
int mac_parse(const char *s, uint8_t out[6]) {
    if (!s || !out) return -1;
    unsigned v[6];
    if (sscanf(s, "%x:%x:%x:%x:%x:%x", &v[0],&v[1],&v[2],&v[3],&v[4],&v[5]) != 6 &&
        sscanf(s, "%x-%x-%x-%x-%x-%x", &v[0],&v[1],&v[2],&v[3],&v[4],&v[5]) != 6)
        return -1;
    for (int i = 0; i < 6; i++) { if (v[i] > 255) return -1; out[i] = (uint8_t)v[i]; }
    return 0;
}
int mac_format(const uint8_t in[6], char *out, int outlen) {
    return snprintf(out, outlen, "%02X:%02X:%02X:%02X:%02X:%02X",
                    in[0], in[1], in[2], in[3], in[4], in[5]);
}
int mac_is_multicast(const uint8_t in[6]) { return in[0] & 1; }
