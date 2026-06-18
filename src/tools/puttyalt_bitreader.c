/* puttyalt_bitreader.c - Read bits MSB-first from a byte buffer.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { const unsigned char *buf; int len; int bitpos; } BitReader;
void br2_init(BitReader *r, const unsigned char *buf, int len) {
    if (!r) return;
    r->buf = buf;
    r->len = len;
    r->bitpos = 0;
}
int br2_read(BitReader *r, int nbits) {
    if (!r || nbits <= 0 || nbits > 24) return -1;
    int v = 0;
    for (int i = 0; i < nbits; i++) {
        int byte = r->bitpos >> 3;
        if (byte >= r->len) return -1;
        int bit = 7 - (r->bitpos & 7);
        v = (v << 1) | ((r->buf[byte] >> bit) & 1);
        r->bitpos++;
    }
    return v;
}
int br2_remaining(const BitReader *r) { return r ? r->len*8 - r->bitpos : 0; }
void br2_align(BitReader *r) { if (r && (r->bitpos & 7)) r->bitpos = (r->bitpos + 7) & ~7; }
