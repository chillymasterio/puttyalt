/* puttyalt_logfmt2.c - Structured logfmt key=value formatting.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
typedef struct { char buf[512]; int len; } LogLine;
void lf3_init(LogLine *l) { if (l) { l->buf[0] = 0; l->len = 0; } }
int lf3_str(LogLine *l, const char *key, const char *val) {
    if (!l || !key || !val) return -1;
    int needs_quote = strchr(val, ' ') || strchr(val, '"') || *val == 0;
    int n = snprintf(l->buf + l->len, sizeof(l->buf) - l->len,
                     "%s%s=%s%s%s", l->len ? " " : "", key,
                     needs_quote ? "\"" : "", val, needs_quote ? "\"" : "");
    if (n < 0 || l->len + n >= (int)sizeof l->buf) return -1;
    l->len += n;
    return 0;
}
int lf3_int(LogLine *l, const char *key, long val) {
    if (!l || !key) return -1;
    int n = snprintf(l->buf + l->len, sizeof(l->buf) - l->len, "%s%s=%ld", l->len ? " " : "", key, val);
    if (n < 0 || l->len + n >= (int)sizeof l->buf) return -1;
    l->len += n;
    return 0;
}
