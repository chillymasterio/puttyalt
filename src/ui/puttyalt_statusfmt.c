/* puttyalt_statusfmt.c - Format status-bar segments.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
typedef struct { char buf[256]; int len; } StatusBar;
void sf3_init(StatusBar *s) { if (s) { s->buf[0] = 0; s->len = 0; } }
int sf3_segment(StatusBar *s, const char *text) {
    if (!s || !text) return -1;
    int n = snprintf(s->buf + s->len, sizeof(s->buf) - s->len, "%s%s",
                     s->len ? " | " : "", text);
    if (n < 0 || s->len + n >= (int)sizeof s->buf) return -1;
    s->len += n;
    return 0;
}
int sf3_keyval(StatusBar *s, const char *key, const char *val) {
    char tmp[96];
    snprintf(tmp, sizeof tmp, "%s: %s", key, val);
    return sf3_segment(s, tmp);
}
