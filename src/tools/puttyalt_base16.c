/* puttyalt_base16.c - Uppercase/lowercase hex with grouping.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int b16_encode(const unsigned char *in, int n, int upper, char *out, int outlen) {
    const char *hex = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    if (!in || !out || outlen < n*2+1) return -1;
    int o=0;
    for (int i=0;i<n;i++){ out[o++]=hex[in[i]>>4]; out[o++]=hex[in[i]&15]; }
    out[o]=0; return o;
}
int b16_grouped(const unsigned char *in, int n, char sep, char *out, int outlen) {
    const char *hex = "0123456789abcdef";
    int o=0;
    for (int i=0;i<n;i++){ if(i&&o<outlen-1)out[o++]=sep; if(o+2>=outlen)return -1; out[o++]=hex[in[i]>>4]; out[o++]=hex[in[i]&15]; }
    out[o]=0; return o;
}
