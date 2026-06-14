/* puttyalt_strtrim2.c - Trim/pad/strip whitespace helpers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
int st2_ltrim(const char *s, char *out, int outlen) {
    if (!s || !out || outlen <= 0) return -1;
    while (*s && isspace((unsigned char)*s)) s++;
    int n = (int)strlen(s);
    if (n >= outlen) n = outlen - 1;
    memcpy(out, s, n); out[n] = 0;
    return n;
}
int st2_rtrim(const char *s, char *out, int outlen) {
    if (!s || !out || outlen <= 0) return -1;
    int n = (int)strlen(s);
    while (n > 0 && isspace((unsigned char)s[n-1])) n--;
    if (n >= outlen) n = outlen - 1;
    memcpy(out, s, n); out[n] = 0;
    return n;
}
int st2_trim(const char *s, char *out, int outlen) {
    char tmp[512];
    if (st2_ltrim(s, tmp, sizeof tmp) < 0) return -1;
    return st2_rtrim(tmp, out, outlen);
}
