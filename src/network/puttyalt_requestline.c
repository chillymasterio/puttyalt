/* puttyalt_requestline.c - Parse HTTP request/status lines.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { char method[12]; char path[256]; char version[16]; } RequestLine;
int rql_parse(const char *line, RequestLine *out) {
    if (!line || !out) return -1;
    int o = 0;
    while (*line && *line != ' ' && o < 11) out->method[o++] = *line++;
    out->method[o] = 0;
    if (*line != ' ') return -1;
    line++;
    o = 0;
    while (*line && *line != ' ' && o < 255) out->path[o++] = *line++;
    out->path[o] = 0;
    if (*line == ' ') line++;
    o = 0;
    while (*line && *line != '\r' && *line != '\n' && o < 15) out->version[o++] = *line++;
    out->version[o] = 0;
    return out->method[0] ? 0 : -1;
}
int rql_status_code(const char *line) {
    if (!line || strncmp(line, "HTTP/", 5) != 0) return -1;
    const char *sp = strchr(line, ' ');
    if (!sp) return -1;
    int code = 0;
    for (sp++; *sp >= '0' && *sp <= '9'; sp++) code = code*10 + (*sp - '0');
    return code;
}
