/* puttyalt_fuzzyscore.c - Score fuzzy substring matches.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
/* Subsequence match with bonus for consecutive + word-start hits.
   Returns score >0 on match, 0 on no match. */
int fz2_score(const char *needle, const char *haystack) {
    if (!needle || !haystack) return 0;
    if (!*needle) return 1;
    int score = 0, streak = 0;
    const char *h = haystack;
    int prev_sep = 1;
    for (const char *n = needle; *n; ) {
        while (*h && tolower((unsigned char)*h) != tolower((unsigned char)*n)) {
            prev_sep = (*h == ' ' || *h == '_' || *h == '-' || *h == '/');
            h++; streak = 0;
        }
        if (!*h) return 0;
        score += 1 + streak;
        if (prev_sep) score += 3;
        streak++;
        prev_sep = 0;
        h++; n++;
    }
    return score;
}
