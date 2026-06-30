/* puttyalt_truncate2.c - Truncate strings with ellipsis.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int tr5_ellipsis(const char *s, int maxlen, char *out, int outlen) {
    if (!s || !out || maxlen < 1 || outlen <= maxlen) return -1;
    int n = (int)strlen(s);
    if (n <= maxlen) { memcpy(out, s, n); out[n] = 0; return n; }
    int keep = maxlen - 3;
    if (keep < 0) keep = 0;
    memcpy(out, s, keep);
    int o = keep;
    for (int i = 0; i < 3 && i < maxlen; i++) out[o++] = '.';
    out[o] = 0;
    return o;
}
/* Truncate in the middle: "long...name". */
int tr5_middle(const char *s, int maxlen, char *out, int outlen) {
    if (!s || !out || maxlen < 5 || outlen <= maxlen) return -1;
    int n = (int)strlen(s);
    if (n <= maxlen) { memcpy(out, s, n); out[n] = 0; return n; }
    int head = (maxlen - 3) / 2, tail = maxlen - 3 - head;
    int o = 0;
    memcpy(out, s, head); o = head;
    out[o++]='.'; out[o++]='.'; out[o++]='.';
    memcpy(out + o, s + n - tail, tail); o += tail;
    out[o] = 0;
    return o;
}
