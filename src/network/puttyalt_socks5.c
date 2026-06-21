/* puttyalt_socks5.c - Build SOCKS5 handshake/request packets.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdint.h>
/* Build a SOCKS5 greeting (no-auth). Returns length. */
int sk5_greeting(uint8_t *out, int cap) {
    if (!out || cap < 3) return -1;
    out[0] = 0x05; out[1] = 0x01; out[2] = 0x00;
    return 3;
}
/* Build a CONNECT request to a domain:port. */
int sk5_connect_domain(const char *host, int port, uint8_t *out, int cap) {
    if (!host || !out) return -1;
    int hl = (int)strlen(host);
    if (hl > 255 || cap < hl + 7) return -1;
    int o = 0;
    out[o++] = 0x05; out[o++] = 0x01; out[o++] = 0x00; out[o++] = 0x03;
    out[o++] = (uint8_t)hl;
    memcpy(out + o, host, hl); o += hl;
    out[o++] = (uint8_t)((port >> 8) & 0xFF);
    out[o++] = (uint8_t)(port & 0xFF);
    return o;
}
int sk5_reply_ok(const uint8_t *resp, int n) {
    return n >= 2 && resp[0] == 0x05 && resp[1] == 0x00;
}
