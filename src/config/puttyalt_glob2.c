/* puttyalt_glob2.c - Glob pattern matcher with * ? [set].
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int glob2_match(const char *pat, const char *str) {
    if (!pat || !str) return 0;
    const char *star = NULL, *ss = NULL;
    while (*str) {
        if (*pat == '?' || *pat == *str) { pat++; str++; }
        else if (*pat == '*') { star = pat++; ss = str; }
        else if (*pat == '[') {
            const char *p = pat + 1; int neg = 0, hit = 0;
            if (*p == '!') { neg = 1; p++; }
            while (*p && *p != ']') {
                if (p[1] == '-' && p[2] && p[2] != ']') {
                    if ((unsigned char)*str >= (unsigned char)p[0] &&
                        (unsigned char)*str <= (unsigned char)p[2]) hit = 1;
                    p += 3;
                } else { if (*str == *p) hit = 1; p++; }
            }
            if (hit != neg && *p == ']') { pat = p + 1; str++; }
            else if (star) { pat = star + 1; str = ++ss; }
            else return 0;
        }
        else if (star) { pat = star + 1; str = ++ss; }
        else return 0;
    }
    while (*pat == '*') pat++;
    return *pat == 0;
}
