/* puttyalt_bitwriter.c - Write bits MSB-first into a byte buffer.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { unsigned char *buf; int cap; int bitpos; } BitWriter;
void bw2_init(BitWriter *w, unsigned char *buf, int cap) {
    if (!w) return;
    w->buf = buf;
    w->cap = cap;
    w->bitpos = 0;
    if (buf) memset(buf, 0, cap);
}
int bw2_write(BitWriter *w, int value, int nbits) {
    if (!w || nbits <= 0 || nbits > 24) return -1;
    for (int i = nbits - 1; i >= 0; i--) {
        int byte = w->bitpos >> 3;
        if (byte >= w->cap) return -1;
        int bit = 7 - (w->bitpos & 7);
        if ((value >> i) & 1) w->buf[byte] |= (unsigned char)(1 << bit);
        w->bitpos++;
    }
    return 0;
}
int bw2_bytes(const BitWriter *w) { return w ? (w->bitpos + 7) >> 3 : 0; }
