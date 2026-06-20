/* puttyalt_boyermoore.c - Boyer-Moore-Horspool search.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int bm3_search(const char *text, const char *pat) {
    if (!text || !pat) return -1;
    int n = (int)strlen(text), m = (int)strlen(pat);
    if (m == 0) return 0;
    if (m > n) return -1;
    int shift[256];
    for (int i = 0; i < 256; i++) shift[i] = m;
    for (int i = 0; i < m - 1; i++) shift[(unsigned char)pat[i]] = m - 1 - i;
    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && text[i+j] == pat[j]) j--;
        if (j < 0) return i;
        i += shift[(unsigned char)text[i+m-1]];
    }
    return -1;
}
int bm3_count(const char *text, const char *pat) {
    if (!text || !pat || !*pat) return 0;
    int count = 0, off = 0, m = (int)strlen(pat);
    int idx;
    while ((idx = bm3_search(text + off, pat)) >= 0) { count++; off += idx + m; }
    return count;
}
