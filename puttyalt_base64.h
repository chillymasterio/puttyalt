#ifndef PUTTYALT_BASE64_H
#define PUTTYALT_BASE64_H

#include <stddef.h>

size_t base64_encode_len(size_t input_len);
int    base64_encode(const unsigned char *input, size_t len,
                     char *output, size_t outlen);
size_t base64_decode_len(const char *input);
int    base64_decode(const char *input, unsigned char *output, size_t outlen);
int    base64_is_valid(const char *input);

#endif
