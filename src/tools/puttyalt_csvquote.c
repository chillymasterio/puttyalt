/* puttyalt_csvquote.c - Quote CSV fields needing escaping.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Quote a CSV field if it contains comma, quote, or newline. */
int cq2_field(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int needs = 0;
    for (const char *p = s; *p; p++)
        if (*p == ',' || *p == '"' || *p == '\n' || *p == '\r') { needs = 1; break; }
    int o = 0;
    if (!needs) {
        int n = (int)strlen(s);
        if (n >= outlen) return -1;
        memcpy(out, s, n); out[n] = 0;
        return n;
    }
    if (o < outlen-1) out[o++] = '"';
    for (const char *p = s; *p && o < outlen-2; p++) {
        if (*p == '"') { if (o < outlen-3) out[o++] = '"'; out[o++] = '"'; }
        else out[o++] = *p;
    }
    if (o < outlen-1) out[o++] = '"';
    out[o] = 0;
    return o;
}
