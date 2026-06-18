/* puttyalt_contenttype.c - Parse Content-Type headers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
typedef struct { char mime[64]; char charset[32]; char boundary[64]; } ContentType;
int ct2_parse(const char *hdr, ContentType *out) {
    if (!hdr || !out) return -1;
    out->mime[0] = out->charset[0] = out->boundary[0] = 0;
    int o = 0;
    while (*hdr && *hdr != ';' && o < 63) out->mime[o++] = (char)tolower((unsigned char)*hdr++);
    out->mime[o] = 0;
    while (*hdr) {
        while (*hdr == ';' || *hdr == ' ') hdr++;
        if (!strncmp(hdr, "charset=", 8)) {
            hdr += 8; o = 0;
            while (*hdr && *hdr != ';' && o < 31) out->charset[o++] = *hdr++;
            out->charset[o] = 0;
        } else if (!strncmp(hdr, "boundary=", 9)) {
            hdr += 9; o = 0;
            while (*hdr && *hdr != ';' && o < 63) out->boundary[o++] = *hdr++;
            out->boundary[o] = 0;
        } else while (*hdr && *hdr != ';') hdr++;
    }
    return out->mime[0] ? 0 : -1;
}
int ct2_is_text(const ContentType *c) {
    return c && (strncmp(c->mime, "text/", 5) == 0 ||
                 strstr(c->mime, "json") || strstr(c->mime, "xml"));
}
