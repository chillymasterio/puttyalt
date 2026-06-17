/* puttyalt_jsonesc.c - Escape/unescape JSON strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int je_escape(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    for (; *s && o < outlen - 2; s++) {
        unsigned char c = (unsigned char)*s;
        switch (c) {
            case '"':  out[o++]='\\'; out[o++]='"'; break;
            case '\\': out[o++]='\\'; out[o++]='\\'; break;
            case '\n': out[o++]='\\'; out[o++]='n'; break;
            case '\t': out[o++]='\\'; out[o++]='t'; break;
            case '\r': out[o++]='\\'; out[o++]='r'; break;
            default:
                if (c < 0x20) { if (o + 6 >= outlen) return -1; o += snprintf(out+o, outlen-o, "\\u%04x", c); }
                else out[o++] = (char)c;
        }
    }
    out[o] = 0;
    return o;
}
int je_unescape(const char *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int o = 0;
    while (*s && o < outlen - 1) {
        if (*s == '\\' && s[1]) {
            s++;
            switch (*s) {
                case 'n': out[o++]='\n'; break;
                case 't': out[o++]='\t'; break;
                case 'r': out[o++]='\r'; break;
                case '"': out[o++]='"'; break;
                case '\\': out[o++]='\\'; break;
                case '/': out[o++]='/'; break;
                default: out[o++]=*s; break;
            }
            s++;
        } else out[o++] = *s++;
    }
    out[o] = 0;
    return o;
}
