/* puttyalt_longestcommon.c - Longest common subsequence length.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int lcs_length(const char *a, const char *b) {
    if (!a || !b) return 0;
    int la = (int)strlen(a), lb = (int)strlen(b);
    if (la > 128) la = 128;
    if (lb > 128) lb = 128;
    int prev[129] = {0}, cur[129];
    for (int i = 1; i <= la; i++) {
        cur[0] = 0;
        for (int j = 1; j <= lb; j++) {
            if (a[i-1] == b[j-1]) cur[j] = prev[j-1] + 1;
            else cur[j] = prev[j] > cur[j-1] ? prev[j] : cur[j-1];
        }
        memcpy(prev, cur, sizeof(int)*(lb+1));
    }
    return prev[lb];
}
/* Longest common substring (contiguous) length. */
int lcs_substring(const char *a, const char *b) {
    if (!a || !b) return 0;
    int la = (int)strlen(a), lb = (int)strlen(b);
    if (la > 128) la = 128;
    if (lb > 128) lb = 128;
    int prev[129] = {0}, cur[129], best = 0;
    for (int i = 1; i <= la; i++) {
        for (int j = 1; j <= lb; j++) {
            if (a[i-1] == b[j-1]) { cur[j] = prev[j-1] + 1; if (cur[j] > best) best = cur[j]; }
            else cur[j] = 0;
        }
        memcpy(prev, cur, sizeof(int)*(lb+1));
    }
    return best;
}
