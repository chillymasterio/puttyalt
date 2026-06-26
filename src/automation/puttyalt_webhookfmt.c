/* puttyalt_webhookfmt.c - Build webhook JSON payloads.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
/* Build a minimal JSON event payload. */
int wh2_event(const char *event, const char *session, long timestamp, char *out, int outlen) {
    if (!event || !out) return -1;
    return snprintf(out, outlen,
        "{\"event\":\"%s\",\"session\":\"%s\",\"timestamp\":%ld}",
        event, session ? session : "", timestamp);
}
/* Escape a value for embedding in JSON. */
int wh2_escape(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s && o < outlen - 2; s++) {
        if (*s == '"' || *s == '\\') { out[o++] = '\\'; out[o++] = *s; }
        else if (*s == '\n') { out[o++] = '\\'; out[o++] = 'n'; }
        else out[o++] = *s;
    }
    out[o] = 0;
    return o;
}
