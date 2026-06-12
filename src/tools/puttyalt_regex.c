#include <string.h>
#include <stdio.h>
/* Simple glob-style pattern matching for terminal highlighting */
int regex_glob_match(const char *pattern, const char *str) {
    while (*pattern && *str) {
        if (*pattern == '*') { pattern++; if (!*pattern) return 1;
            while (*str) { if (regex_glob_match(pattern, str)) return 1; str++; } return 0; }
        if (*pattern == '?') { pattern++; str++; continue; }
        if (*pattern != *str) return 0;
        pattern++; str++;
    }
    while (*pattern == '*') pattern++;
    return !*pattern && !*str;
}
int regex_find_all(const char *text, const char *pattern, int positions[], int max) {
    int n = 0, tlen = (int)strlen(text), plen = (int)strlen(pattern);
    for (int i = 0; i <= tlen - plen && n < max; i++)
        if (strncmp(text + i, pattern, plen) == 0) positions[n++] = i;
    return n;
}
