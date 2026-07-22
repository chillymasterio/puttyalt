/* puttyalt_utf16len.c - Count UTF-16 code units for a UTF-8 buffer. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int utf16len_apply(const char *in, char *out, int outlen) {
    if (!in || !out || outlen <= 0) return -1;
    int i = 0;
    for (const char *p = in; *p && i < outlen - 1; ++p) {
        char c = *p;
        if (c == '\\' || c == '/' || c == '.' || (c >= 0x20 && c < 0x7F)) {
            out[i++] = c;
        }
    }
    out[i] = '\0';
    return i;
}
int utf16len_length(const char *s) {
    return s ? (int)strlen(s) : 0;
}
int utf16len_first_of(const char *s, const char *set) {
    if (!s || !set) return -1;
    for (int i = 0; s[i]; i++) if (strchr(set, s[i])) return i;
    return -1;
}
