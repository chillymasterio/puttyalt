/* puttyalt_cidr4.c - Parse and match IPv4 CIDR ranges.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <stdint.h>
int cidr_parse(const char *s, uint32_t *net, int *bits) {
    if (!s || !net || !bits) return -1;
    unsigned a, b, c, d, m;
    if (sscanf(s, "%u.%u.%u.%u/%u", &a, &b, &c, &d, &m) != 5) return -1;
    if (a > 255 || b > 255 || c > 255 || d > 255 || m > 32) return -1;
    *net = (a << 24) | (b << 16) | (c << 8) | d;
    *bits = (int)m;
    return 0;
}
int cidr_contains(uint32_t net, int bits, uint32_t ip) {
    if (bits <= 0) return 1;
    if (bits > 32) return 0;
    uint32_t mask = (bits == 32) ? 0xFFFFFFFFu : ~((1u << (32 - bits)) - 1);
    return (net & mask) == (ip & mask);
}
int cidr_ip(const char *s, uint32_t *ip) {
    unsigned a, b, c, d;
    if (sscanf(s, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) return -1;
    if (a > 255 || b > 255 || c > 255 || d > 255) return -1;
    *ip = (a << 24) | (b << 16) | (c << 8) | d;
    return 0;
}
