/* puttyalt_envfile.c - Parse .env style KEY=VALUE files.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
/* Find the value for `key` in a multi-line .env buffer. Returns len or -1. */
int envf_get(const char *buf, const char *key, char *out, int outlen) {
    if (!buf || !key || !out) return -1;
    int klen = (int)strlen(key);
    const char *p = buf;
    while (*p) {
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '#') { while (*p && *p != '\n') p++; if (*p) p++; continue; }
        if (strncmp(p, key, klen) == 0 && (p[klen] == '=' )) {
            const char *v = p + klen + 1;
            int o = 0;
            if (*v == '"') { v++; while (*v && *v != '"' && o < outlen-1) out[o++] = *v++; }
            else while (*v && *v != '\n' && *v != '\r' && o < outlen-1) out[o++] = *v++;
            out[o] = 0;
            return o;
        }
        while (*p && *p != '\n') p++;
        if (*p) p++;
    }
    return -1;
}
int envf_count(const char *buf) {
    if (!buf) return 0;
    int n = 0; const char *p = buf;
    while (*p) {
        while (*p == ' ' || *p == '\t') p++;
        if (*p && *p != '#' && *p != '\n' && strchr(p, '=')) {
            const char *eol = strchr(p, '\n');
            const char *eq = strchr(p, '=');
            if (eq && (!eol || eq < eol)) n++;
        }
        while (*p && *p != '\n') p++;
        if (*p) p++;
    }
    return n;
}
