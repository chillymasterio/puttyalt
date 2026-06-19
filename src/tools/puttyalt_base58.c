/* puttyalt_base58.c - Base58 encode (Bitcoin alphabet).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static const char B58[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
int b58_encode(const unsigned char *in, int n, char *out, int outlen) {
    if (!in || !out || n <= 0) return -1;
    unsigned char tmp[256];
    if (n > 200) return -1;
    int zeros = 0;
    while (zeros < n && in[zeros] == 0) zeros++;
    int len = 0;
    unsigned char buf[400]; memset(buf, 0, sizeof buf);
    for (int i = 0; i < n; i++) {
        int carry = in[i];
        for (int j = 0; j < len || carry; j++) {
            if (j < (int)sizeof buf) {
                carry += 256 * buf[j];
                buf[j] = carry % 58;
                carry /= 58;
                if (j >= len) len = j + 1;
            }
        }
    }
    (void)tmp;
    int o = 0;
    for (int i = 0; i < zeros && o < outlen-1; i++) out[o++] = '1';
    for (int i = len - 1; i >= 0 && o < outlen-1; i--) out[o++] = B58[buf[i]];
    out[o] = 0;
    return o;
}
