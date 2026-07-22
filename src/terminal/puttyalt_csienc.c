/* puttyalt_csienc.c - Build a CSI escape sequence from parameters. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int csienc_written(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int n = snprintf(out, outlen, "%s", in);
    return (n < 0 || n >= outlen) ? -1 : n;
}
int csienc_render(const char *in, int width, char *out, int outlen) {
    if (!in || !out || outlen <= 0 || width <= 0) return -1;
    int len = (int)strlen(in);
    if (len > width) len = width;
    if (len >= outlen) len = outlen - 1;
    memcpy(out, in, (size_t)len);
    out[len] = '\0';
    return len;
}
int csienc_pad(char *buf, int buflen, int width, char fill) {
    if (!buf || buflen <= 0 || width <= 0) return -1;
    int len = (int)strlen(buf);
    while (len < width && len < buflen - 1) buf[len++] = fill;
    buf[len] = '\0';
    return len;
}
