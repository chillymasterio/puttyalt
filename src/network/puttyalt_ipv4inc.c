/* puttyalt_ipv4inc.c - Increment and iterate IPv4 addresses.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <stdio.h>
uint32_t i4i_next(uint32_t ip) { return ip + 1; }
uint32_t i4i_prev(uint32_t ip) { return ip - 1; }
/* Distance between two IPs (b - a). */
long i4i_distance(uint32_t a, uint32_t b) { return (long)b - (long)a; }
/* Nth address in a range starting at base. */
uint32_t i4i_offset(uint32_t base, int n) { return base + n; }
int i4i_format(uint32_t ip, char *out, int outlen) {
    return snprintf(out, outlen, "%u.%u.%u.%u", (ip>>24)&255, (ip>>16)&255, (ip>>8)&255, ip&255);
}
