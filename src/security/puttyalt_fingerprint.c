#include <string.h>
#include <stdio.h>

int fingerprint_format_sha256(const unsigned char *hash, int len, char *out, int outlen)
{
    if (len <= 0 || !out) return -1;
    int pos = 0;
    for (int i = 0; i < len && pos < outlen - 3; i++) {
        if (i > 0) out[pos++] = ':';
        pos += snprintf(out + pos, outlen - pos, "%02x", hash[i]);
    }
    return pos;
}

int fingerprint_format_md5(const unsigned char *hash, int len, char *out, int outlen)
{
    if (len != 16 || !out) return -1;
    int pos = 0;
    for (int i = 0; i < 16 && pos < outlen - 3; i++) {
        if (i > 0) out[pos++] = ':';
        pos += snprintf(out + pos, outlen - pos, "%02x", hash[i]);
    }
    return pos;
}

int fingerprint_randomart(const unsigned char *hash, int len, char *out, int outlen)
{
    /* Bishop's randomart visualization */
    int field[9][17];
    memset(field, 0, sizeof(field));
    int x = 8, y = 4;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 4; j++) {
            int v = (hash[i] >> (2 * j)) & 3;
            if (v & 1) x = x < 16 ? x + 1 : x; else x = x > 0 ? x - 1 : x;
            if (v & 2) y = y < 8 ? y + 1 : y; else y = y > 0 ? y - 1 : y;
            field[y][x]++;
        }
    }
    const char augment[] = " .o+=*BOX@%&#/^SE";
    int pos = 0;
    pos += snprintf(out + pos, outlen - pos, "+---[PuttyAlt]----+\n");
    for (int r = 0; r < 9 && pos < outlen - 20; r++) {
        out[pos++] = '|';
        for (int c = 0; c < 17; c++) {
            int v = field[r][c];
            if (v >= 16) v = 16;
            out[pos++] = augment[v];
        }
        out[pos++] = '|'; out[pos++] = '\n';
    }
    pos += snprintf(out + pos, outlen - pos, "+-----------------+");
    return pos;
}
