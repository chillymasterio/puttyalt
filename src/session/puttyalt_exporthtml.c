/* puttyalt_exporthtml.c - Export session log to HTML.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
int ehtml_header(char *out, int outlen, const char *title) {
    return snprintf(out, outlen,
        "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
        "<title>%s</title><style>body{background:#0d1117;color:#c9d1d9;"
        "font-family:monospace}</style></head><body><pre>\n", title ? title : "Session");
}
int ehtml_escape_line(const char *line, char *out, int outlen) {
    if (!line || !out) return -1;
    int o = 0;
    for (const char *p = line; *p && o < outlen - 7; p++) {
        if (*p == '<') { memcpy(out+o, "&lt;", 4); o += 4; }
        else if (*p == '>') { memcpy(out+o, "&gt;", 4); o += 4; }
        else if (*p == '&') { memcpy(out+o, "&amp;", 5); o += 5; }
        else out[o++] = *p;
    }
    out[o] = 0;
    return o;
}
int ehtml_footer(char *out, int outlen) {
    return snprintf(out, outlen, "</pre></body></html>\n");
}
