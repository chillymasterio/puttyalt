/* puttyalt_ipv6class.c - Classify IPv6 address types.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int i6c_is_loopback(const uint8_t a[16]) {
    for (int i = 0; i < 15; i++) if (a[i]) return 0;
    return a[15] == 1;
}
int i6c_is_linklocal(const uint8_t a[16]) { return a[0] == 0xfe && (a[1] & 0xc0) == 0x80; }
int i6c_is_multicast(const uint8_t a[16]) { return a[0] == 0xff; }
int i6c_is_unspecified(const uint8_t a[16]) {
    for (int i = 0; i < 16; i++) if (a[i]) return 0;
    return 1;
}
int i6c_is_v4mapped(const uint8_t a[16]) {
    for (int i = 0; i < 10; i++) if (a[i]) return 0;
    return a[10] == 0xff && a[11] == 0xff;
}
const char *i6c_scope(const uint8_t a[16]) {
    if (i6c_is_loopback(a)) return "loopback";
    if (i6c_is_linklocal(a)) return "link-local";
    if (i6c_is_multicast(a)) return "multicast";
    if (i6c_is_v4mapped(a)) return "v4-mapped";
    if (i6c_is_unspecified(a)) return "unspecified";
    return "global";
}
