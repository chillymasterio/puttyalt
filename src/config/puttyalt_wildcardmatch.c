/* puttyalt_wildcardmatch.c - Match hostnames against wildcard patterns.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Match a hostname against a pattern like "*.example.com". */
int wm3_match(const char *pattern, const char *host) {
    if (!pattern || !host) return 0;
    if (pattern[0] == '*' && pattern[1] == '.') {
        const char *dot = strchr(host, '.');
        if (!dot) return 0;
        return strcmp(pattern + 2, dot + 1) == 0;
    }
    return strcmp(pattern, host) == 0;
}
/* Match against a comma-separated pattern list. */
int wm3_match_list(const char *patterns, const char *host) {
    if (!patterns || !host) return 0;
    char buf[256];
    strncpy(buf, patterns, 255); buf[255] = 0;
    char *tok = buf;
    while (tok && *tok) {
        char *comma = strchr(tok, ',');
        if (comma) *comma = 0;
        while (*tok == ' ') tok++;
        if (wm3_match(tok, host)) return 1;
        if (!comma) break;
        tok = comma + 1;
    }
    return 0;
}
