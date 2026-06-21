/* puttyalt_ipclass.c - Classify IPv4 addresses (private/loopback).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int ic2_is_private(uint32_t ip) {
    uint8_t a = (ip>>24)&255, b = (ip>>16)&255;
    if (a == 10) return 1;
    if (a == 172 && b >= 16 && b <= 31) return 1;
    if (a == 192 && b == 168) return 1;
    return 0;
}
int ic2_is_loopback(uint32_t ip) { return ((ip>>24)&255) == 127; }
int ic2_is_multicast(uint32_t ip) { uint8_t a=(ip>>24)&255; return a >= 224 && a <= 239; }
int ic2_is_linklocal(uint32_t ip) { return ((ip>>24)&255) == 169 && ((ip>>16)&255) == 254; }
const char *ic2_scope(uint32_t ip) {
    if (ic2_is_loopback(ip)) return "loopback";
    if (ic2_is_private(ip)) return "private";
    if (ic2_is_multicast(ip)) return "multicast";
    if (ic2_is_linklocal(ip)) return "link-local";
    return "public";
}
