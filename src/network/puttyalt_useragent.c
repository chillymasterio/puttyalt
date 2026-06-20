/* puttyalt_useragent.c - Build and parse User-Agent strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
int ua2_build(const char *product, const char *version, const char *platform, char *out, int outlen) {
    if (!product || !version || !out) return -1;
    return snprintf(out, outlen, "%s/%s (%s)", product, version, platform ? platform : "unknown");
}
/* Extract the product name (before first '/'). */
int ua2_product(const char *ua, char *out, int outlen) {
    if (!ua || !out) return -1;
    int o = 0;
    while (ua[o] && ua[o] != '/' && o < outlen - 1) { out[o] = ua[o]; o++; }
    out[o] = 0;
    return o;
}
int ua2_is_bot(const char *ua) {
    if (!ua) return 0;
    return strstr(ua, "bot") || strstr(ua, "Bot") || strstr(ua, "crawler") || strstr(ua, "spider");
}
