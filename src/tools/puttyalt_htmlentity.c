/* puttyalt_htmlentity.c - Encode/decode common HTML entities.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int he2_encode(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s && o < outlen - 7; s++) {
        switch (*s) {
            case '&': memcpy(out+o, "&amp;", 5); o += 5; break;
            case '<': memcpy(out+o, "&lt;", 4); o += 4; break;
            case '>': memcpy(out+o, "&gt;", 4); o += 4; break;
            case '"': memcpy(out+o, "&quot;", 6); o += 6; break;
            case '\'': memcpy(out+o, "&#39;", 5); o += 5; break;
            default: out[o++] = *s;
        }
    }
    out[o] = 0;
    return o;
}
int he2_decode(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    while (*s && o < outlen - 1) {
        if (*s == '&') {
            if (!strncmp(s, "&amp;", 5)) { out[o++]='&'; s += 5; continue; }
            if (!strncmp(s, "&lt;", 4)) { out[o++]='<'; s += 4; continue; }
            if (!strncmp(s, "&gt;", 4)) { out[o++]='>'; s += 4; continue; }
            if (!strncmp(s, "&quot;", 6)) { out[o++]='"'; s += 6; continue; }
            if (!strncmp(s, "&#39;", 5)) { out[o++]='\''; s += 5; continue; }
        }
        out[o++] = *s++;
    }
    out[o] = 0;
    return o;
}
