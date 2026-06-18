/* puttyalt_hufflen.c - Build canonical Huffman code lengths.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define HL_SYM 64
/* Given symbol frequencies, assign code lengths via a simple package-merge
   approximation (length-limited). Writes lengths into `lens`. */
int hl_build(const int *freq, int n, int maxlen, unsigned char *lens) {
    if (!freq || !lens || n <= 0 || n > HL_SYM) return -1;
    /* sort indices by frequency descending */
    int idx[HL_SYM];
    for (int i = 0; i < n; i++) idx[i] = i;
    for (int i = 0; i < n; i++) for (int j = i+1; j < n; j++)
        if (freq[idx[j]] > freq[idx[i]]) { int t = idx[i]; idx[i] = idx[j]; idx[j] = t; }
    /* assign increasing lengths, capped at maxlen */
    for (int i = 0; i < n; i++) {
        int rank = i;
        int l = 1;
        while ((1 << l) <= rank + 1 && l < maxlen) l++;
        lens[idx[i]] = (unsigned char)(freq[idx[i]] > 0 ? l : 0);
    }
    return 0;
}
int hl_total_bits(const int *freq, const unsigned char *lens, int n) {
    int total = 0;
    for (int i = 0; i < n; i++) total += freq[i] * lens[i];
    return total;
}
