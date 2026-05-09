#include <string.h>
#include "puttyalt_base64.h"

static const char b64chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int b64val(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

size_t base64_encode_len(size_t input_len)
{
    return ((input_len + 2) / 3) * 4 + 1;
}

int base64_encode(const unsigned char *input, size_t len,
                  char *output, size_t outlen)
{
    if (outlen < base64_encode_len(len)) return -1;
    size_t o = 0;
    for (size_t i = 0; i < len; i += 3) {
        unsigned int n = ((unsigned int)input[i]) << 16;
        if (i + 1 < len) n |= ((unsigned int)input[i+1]) << 8;
        if (i + 2 < len) n |= (unsigned int)input[i+2];

        output[o++] = b64chars[(n >> 18) & 0x3F];
        output[o++] = b64chars[(n >> 12) & 0x3F];
        output[o++] = (i + 1 < len) ? b64chars[(n >> 6) & 0x3F] : '=';
        output[o++] = (i + 2 < len) ? b64chars[n & 0x3F] : '=';
    }
    output[o] = '\0';
    return (int)o;
}

size_t base64_decode_len(const char *input)
{
    size_t len = strlen(input);
    size_t pad = 0;
    if (len >= 2 && input[len-1] == '=') pad++;
    if (len >= 2 && input[len-2] == '=') pad++;
    return (len / 4) * 3 - pad;
}

int base64_decode(const char *input, unsigned char *output, size_t outlen)
{
    size_t len = strlen(input);
    if (len % 4 != 0) return -1;
    size_t expected = base64_decode_len(input);
    if (outlen < expected) return -1;

    size_t o = 0;
    for (size_t i = 0; i < len; i += 4) {
        int a = b64val(input[i]), b = b64val(input[i+1]);
        int c = input[i+2] == '=' ? 0 : b64val(input[i+2]);
        int d = input[i+3] == '=' ? 0 : b64val(input[i+3]);
        if (a < 0 || b < 0) return -1;

        unsigned int n = (a << 18) | (b << 12) | (c << 6) | d;
        output[o++] = (n >> 16) & 0xFF;
        if (input[i+2] != '=') output[o++] = (n >> 8) & 0xFF;
        if (input[i+3] != '=') output[o++] = n & 0xFF;
    }
    return (int)o;
}

int base64_is_valid(const char *input)
{
    size_t len = strlen(input);
    if (len % 4 != 0) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '=') {
            if (i < len - 2) return 0;
        } else if (b64val(input[i]) < 0) return 0;
    }
    return 1;
}
