/* puttyalt_stringbuilder.c - Growable string builder over fixed buffer.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
#define SBLD_CAP 2048
typedef struct { char buf[SBLD_CAP]; int len; int truncated; } StringBuilder;
void sb5_init(StringBuilder *s) { if (s) { s->buf[0] = 0; s->len = 0; s->truncated = 0; } }
int sb5_append(StringBuilder *s, const char *str) {
    if (!s || !str) return -1;
    int n = (int)strlen(str);
    if (s->len + n >= SBLD_CAP) { n = SBLD_CAP - s->len - 1; s->truncated = 1; }
    memcpy(s->buf + s->len, str, n);
    s->len += n; s->buf[s->len] = 0;
    return n;
}
int sb5_append_int(StringBuilder *s, long v) {
    char tmp[24];
    snprintf(tmp, sizeof tmp, "%ld", v);
    return sb5_append(s, tmp);
}
int sb5_append_char(StringBuilder *s, char c) {
    if (!s || s->len >= SBLD_CAP - 1) { if (s) s->truncated = 1; return -1; }
    s->buf[s->len++] = c; s->buf[s->len] = 0;
    return 0;
}
const char *sb5_str(const StringBuilder *s) { return s ? s->buf : ""; }
