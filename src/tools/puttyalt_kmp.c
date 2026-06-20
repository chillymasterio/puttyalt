/* puttyalt_kmp.c - Knuth-Morris-Pratt substring search.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Build the KMP failure table for pattern into `lps`. */
void kmp2_table(const char *pat, int m, int *lps) {
    lps[0] = 0;
    int len = 0, i = 1;
    while (i < m) {
        if (pat[i] == pat[len]) lps[i++] = ++len;
        else if (len) len = lps[len-1];
        else lps[i++] = 0;
    }
}
/* Return first index of pat in text, or -1. */
int kmp2_search(const char *text, const char *pat) {
    if (!text || !pat) return -1;
    int n = (int)strlen(text), m = (int)strlen(pat);
    if (m == 0) return 0;
    if (m > 256) return -1;
    int lps[256];
    kmp2_table(pat, m, lps);
    int i = 0, j = 0;
    while (i < n) {
        if (text[i] == pat[j]) { i++; j++; if (j == m) return i - m; }
        else if (j) j = lps[j-1];
        else i++;
    }
    return -1;
}
