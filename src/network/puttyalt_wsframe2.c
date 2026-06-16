/* puttyalt_wsframe2.c - Encode/decode WebSocket frame headers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
typedef struct { int fin; int opcode; int masked; long payload_len; int header_len; } WsHeader;
/* Parse a frame header from raw bytes. Returns 0 if complete, -1 if need more. */
int wsf_parse(const unsigned char *d, int n, WsHeader *h) {
    if (!d || !h || n < 2) return -1;
    h->fin = (d[0] & 0x80) ? 1 : 0;
    h->opcode = d[0] & 0x0F;
    h->masked = (d[1] & 0x80) ? 1 : 0;
    long len = d[1] & 0x7F;
    int off = 2;
    if (len == 126) {
        if (n < 4) return -1;
        len = ((long)d[2] << 8) | d[3]; off = 4;
    } else if (len == 127) {
        if (n < 10) return -1;
        len = 0;
        for (int i = 0; i < 8; i++) len = (len << 8) | d[2 + i];
        off = 10;
    }
    if (h->masked) off += 4;
    h->payload_len = len;
    h->header_len = off;
    return 0;
}
int wsf_build(int fin, int opcode, long len, unsigned char *out, int outcap) {
    if (!out || outcap < 2) return -1;
    out[0] = (unsigned char)((fin ? 0x80 : 0) | (opcode & 0x0F));
    if (len < 126) { out[1] = (unsigned char)len; return 2; }
    if (len < 65536) {
        if (outcap < 4) return -1;
        out[1] = 126; out[2] = (unsigned char)(len >> 8); out[3] = (unsigned char)len; return 4;
    }
    if (outcap < 10) return -1;
    out[1] = 127;
    for (int i = 0; i < 8; i++) out[2 + i] = (unsigned char)(len >> (56 - i * 8));
    return 10;
}
