#include <string.h>
#include <stdio.h>
int hexdump_format(const unsigned char *data, int len, char *out, int outlen) {
    int pos = 0;
    for (int i = 0; i < len && pos < outlen - 80; i += 16) {
        pos += snprintf(out + pos, outlen - pos, "%08x  ", i);
        for (int j = 0; j < 16; j++) {
            if (i + j < len) pos += snprintf(out + pos, outlen - pos, "%02x ", data[i+j]);
            else pos += snprintf(out + pos, outlen - pos, "   ");
            if (j == 7) out[pos++] = ' ';
        }
        pos += snprintf(out + pos, outlen - pos, " |");
        for (int j = 0; j < 16 && i + j < len; j++) {
            unsigned char c = data[i+j];
            out[pos++] = (c >= 32 && c < 127) ? c : '.';
        }
        pos += snprintf(out + pos, outlen - pos, "|\n");
    }
    out[pos] = 0; return pos;
}
