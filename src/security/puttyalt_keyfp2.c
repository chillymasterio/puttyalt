/* puttyalt_keyfp2.c - SSH key fingerprint formatting (SHA256 base64 / MD5 hex). */
#include <string.h>
#include <stdio.h>
int keyfp2_format_md5(const unsigned char *hash, int len, char *out, int outlen) {
    if (!hash || !out) return -1;
    int pos = 0;
    for (int i = 0; i < len && pos < outlen - 3; i++) {
        pos += snprintf(out + pos, outlen - pos, "%s%02x", i > 0 ? ":" : "", hash[i]);
    }
    return pos;
}
static const char B64C[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int keyfp2_format_sha256(const unsigned char *hash, int len, char *out, int outlen) {
    if (!hash || !out) return -1;
    int o = 0;
    o += snprintf(out + o, outlen - o, "SHA256:");
    int bits = 0;
    unsigned long buf = 0;
    for (int i = 0; i < len; i++) {
        buf = (buf << 8) | hash[i];
        bits += 8;
        while (bits >= 6 && o < outlen - 1) {
            out[o++] = B64C[(buf >> (bits - 6)) & 0x3F];
            bits -= 6;
        }
    }
    if (bits > 0 && o < outlen - 1) {
        out[o++] = B64C[(buf << (6 - bits)) & 0x3F];
    }
    out[o] = 0;
    return o;
}
int keyfp2_compare(const char *fp1, const char *fp2) {
    if (!fp1 || !fp2) return -1;
    return strcmp(fp1, fp2) == 0 ? 1 : 0;
}
