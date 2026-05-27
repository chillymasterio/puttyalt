#include <string.h>
/* Simple RLE compression for session data transfer */
int compress_rle(const unsigned char *in, int inlen, unsigned char *out, int outlen) {
    int i = 0, o = 0;
    while (i < inlen && o + 2 < outlen) {
        unsigned char c = in[i]; int run = 1;
        while (i + run < inlen && in[i + run] == c && run < 255) run++;
        if (run >= 3) { out[o++] = 0xFF; out[o++] = (unsigned char)run; out[o++] = c; }
        else { for (int j = 0; j < run && o < outlen; j++) { if (c == 0xFF && o + 1 < outlen) { out[o++] = 0xFF; out[o++] = 1; out[o++] = c; } else out[o++] = c; } }
        i += run;
    }
    return o;
}
int decompress_rle(const unsigned char *in, int inlen, unsigned char *out, int outlen) {
    int i = 0, o = 0;
    while (i < inlen && o < outlen) {
        if (in[i] == 0xFF && i + 2 < inlen) { int run = in[i+1]; unsigned char c = in[i+2];
            for (int j = 0; j < run && o < outlen; j++) out[o++] = c; i += 3; }
        else out[o++] = in[i++];
    }
    return o;
}
