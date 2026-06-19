/* puttyalt_subnet.c - IPv4 subnet calculations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <stdio.h>
uint32_t sn2_netmask(int prefix) {
    if (prefix <= 0) return 0;
    if (prefix >= 32) return 0xFFFFFFFFu;
    return ~((1u << (32 - prefix)) - 1);
}
uint32_t sn2_network(uint32_t ip, int prefix) { return ip & sn2_netmask(prefix); }
uint32_t sn2_broadcast(uint32_t ip, int prefix) { return ip | ~sn2_netmask(prefix); }
long sn2_host_count(int prefix) {
    if (prefix >= 31) return prefix == 32 ? 1 : 2;
    return (1L << (32 - prefix)) - 2;
}
int sn2_format(uint32_t ip, char *out, int outlen) {
    return snprintf(out, outlen, "%u.%u.%u.%u",
        (ip>>24)&255, (ip>>16)&255, (ip>>8)&255, ip&255);
}
