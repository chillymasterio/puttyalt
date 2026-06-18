/* puttyalt_deltaenc.c - Delta encode/decode numeric streams.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Replace values with successive deltas (in place returns via out). */
int de2_encode(const int *in, int n, int *out) {
    if (!in || !out || n <= 0) return -1;
    int prev = 0;
    for (int i = 0; i < n; i++) { out[i] = in[i] - prev; prev = in[i]; }
    return 0;
}
int de2_decode(const int *in, int n, int *out) {
    if (!in || !out || n <= 0) return -1;
    int acc = 0;
    for (int i = 0; i < n; i++) { acc += in[i]; out[i] = acc; }
    return 0;
}
/* Zigzag map signed->unsigned for compact varint storage. */
unsigned de2_zigzag(int v) { return (unsigned)((v << 1) ^ (v >> 31)); }
int de2_unzigzag(unsigned v) { return (int)((v >> 1) ^ (~(v & 1) + 1)); }
