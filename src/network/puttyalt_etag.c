/* puttyalt_etag.c - Generate and compare HTTP ETags.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
/* Compute a weak ETag from content length + a 32-bit hash. */
uint32_t et_hash(const unsigned char *data, int n) {
    uint32_t h = 2166136261u;
    for (int i = 0; i < n; i++) { h ^= data[i]; h *= 16777619u; }
    return h;
}
int et_format(uint32_t hash, long len, char *out, int outlen) {
    return snprintf(out, outlen, "\"%lx-%x\"", len, hash);
}
/* Compare a client If-None-Match value against our ETag (ignoring W/). */
int et_matches(const char *etag, const char *if_none_match) {
    if (!etag || !if_none_match) return 0;
    if (if_none_match[0] == 'W' && if_none_match[1] == '/') if_none_match += 2;
    if (etag[0] == 'W' && etag[1] == '/') etag += 2;
    if (if_none_match[0] == '*' ) return 1;
    return strcmp(etag, if_none_match) == 0;
}
