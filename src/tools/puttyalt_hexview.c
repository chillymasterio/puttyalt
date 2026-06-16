/* puttyalt_hexview.c - Format hex+ASCII view of a buffer.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <ctype.h>
/* Format one 16-byte row at offset. Returns chars written. */
int hv_row(const unsigned char *data, int len, int offset, char *out, int outlen) {
    if (!data || !out) return -1;
    int o = snprintf(out, outlen, "%08x  ", offset);
    for (int i = 0; i < 16; i++) {
        if (i < len) o += snprintf(out + o, outlen - o, "%02x ", data[offset + i]);
        else o += snprintf(out + o, outlen - o, "   ");
        if (i == 7) o += snprintf(out + o, outlen - o, " ");
    }
    o += snprintf(out + o, outlen - o, " |");
    for (int i = 0; i < 16 && i < len; i++) {
        unsigned char c = data[offset + i];
        out[o++] = isprint(c) ? (char)c : '.';
    }
    o += snprintf(out + o, outlen - o, "|");
    return o;
}
int hv_rows_needed(int len) { return (len + 15) / 16; }
