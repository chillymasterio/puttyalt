/* Simple RLE compression for scrollback buffer.
 * Terminal output has lots of repeated spaces and escape sequences,
 * making RLE effective for ~30-50% compression. */
#include <string.h>
#include <stdlib.h>
#include "puttyalt_scrollcomp.h"

int scroll_compress(CompressedScrollback *cs, const char *raw, size_t len)
{
    memset(cs, 0, sizeof(*cs));
    if (!raw || len == 0) return -1;

    /* Worst case: no compression, output = 2 * input */
    cs->data = (unsigned char *)malloc(len * 2);
    if (!cs->data) return -1;

    size_t out = 0;
    size_t i = 0;
    while (i < len) {
        /* Count run of same byte */
        unsigned char c = (unsigned char)raw[i];
        size_t run = 1;
        while (i + run < len && raw[i + run] == c && run < 127)
            run++;

        if (run >= 3) {
            /* RLE: 0x80 | count, byte */
            cs->data[out++] = 0x80 | (unsigned char)run;
            cs->data[out++] = c;
            if (c == '\n') cs->num_lines += run;
            i += run;
        } else {
            /* Literal */
            cs->data[out++] = c;
            if (c == '\n') cs->num_lines++;
            i++;
        }
    }

    cs->compressed_size = out;
    cs->original_size = len;
    return 0;
}

int scroll_decompress(const CompressedScrollback *cs, char *out, size_t outlen)
{
    if (!cs->data || cs->compressed_size == 0) return -1;

    size_t op = 0;
    for (size_t i = 0; i < cs->compressed_size && op < outlen; i++) {
        if (cs->data[i] & 0x80) {
            int count = cs->data[i] & 0x7F;
            i++;
            if (i >= cs->compressed_size) break;
            unsigned char c = cs->data[i];
            for (int j = 0; j < count && op < outlen; j++)
                out[op++] = c;
        } else {
            out[op++] = cs->data[i];
        }
    }
    return (int)op;
}

void scroll_free(CompressedScrollback *cs)
{
    free(cs->data);
    cs->data = NULL;
    cs->compressed_size = 0;
}

double scroll_ratio(const CompressedScrollback *cs)
{
    if (cs->original_size == 0) return 0;
    return 1.0 - ((double)cs->compressed_size / cs->original_size);
}
