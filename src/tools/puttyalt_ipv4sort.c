/* puttyalt_ipv4sort.c - Sort and dedup IPv4 addresses.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
void i4s_sort(uint32_t *ips, int n) {
    for (int i = 0; i < n; i++) for (int j = i+1; j < n; j++)
        if (ips[j] < ips[i]) { uint32_t t = ips[i]; ips[i] = ips[j]; ips[j] = t; }
}
/* Sort then remove duplicates in place; returns new count. */
int i4s_dedup(uint32_t *ips, int n) {
    if (n <= 1) return n;
    i4s_sort(ips, n);
    int w = 1;
    for (int i = 1; i < n; i++) if (ips[i] != ips[w-1]) ips[w++] = ips[i];
    return w;
}
