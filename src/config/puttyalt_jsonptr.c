/* puttyalt_jsonptr.c - Resolve JSON Pointer (RFC 6901) paths.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Split a JSON Pointer like "/a/b/0" into tokens. Returns token count. */
int jptr_split(const char *ptr, char tokens[][64], int maxtok) {
    if (!ptr || ptr[0] != '/') return ptr && ptr[0]==0 ? 0 : -1;
    int nt = 0;
    const char *p = ptr + 1;
    while (nt < maxtok) {
        int o = 0;
        while (*p && *p != '/' && o < 63) {
            if (*p == '~' && p[1] == '0') { tokens[nt][o++] = '~'; p += 2; }
            else if (*p == '~' && p[1] == '1') { tokens[nt][o++] = '/'; p += 2; }
            else tokens[nt][o++] = *p++;
        }
        tokens[nt][o] = 0;
        nt++;
        if (*p != '/') break;
        p++;
    }
    return nt;
}
int jptr_is_index(const char *token) {
    if (!token || !*token) return 0;
    for (const char *p = token; *p; p++) if (*p < '0' || *p > '9') return 0;
    return 1;
}
