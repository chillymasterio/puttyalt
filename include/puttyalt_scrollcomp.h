#ifndef PUTTYALT_SCROLLCOMP_H
#define PUTTYALT_SCROLLCOMP_H

#include <stddef.h>

typedef struct CompressedScrollback {
    unsigned char *data;
    size_t compressed_size;
    size_t original_size;
    int num_lines;
} CompressedScrollback;

int  scroll_compress(CompressedScrollback *cs, const char *raw, size_t len);
int  scroll_decompress(const CompressedScrollback *cs, char *out, size_t outlen);
void scroll_free(CompressedScrollback *cs);
double scroll_ratio(const CompressedScrollback *cs);

#endif
