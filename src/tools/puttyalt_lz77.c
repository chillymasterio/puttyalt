/* puttyalt_lz77.c - Minimal LZ77 sliding-window matcher.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Find the longest prior match for the lookahead at `pos`. Window is the
   preceding bytes. Returns match length (>=0); sets *dist to back-distance. */
int lz_longest_match(const unsigned char *buf, int pos, int len, int window, int *dist) {
    if (!buf || pos <= 0 || pos >= len) { if (dist) *dist = 0; return 0; }
    int best_len = 0, best_dist = 0;
    int start = pos - window; if (start < 0) start = 0;
    for (int j = start; j < pos; j++) {
        int k = 0;
        while (pos + k < len && buf[j + k] == buf[pos + k] && k < 255) {
            k++;
            if (j + k >= pos) break; /* don't overlap past current */
        }
        if (k > best_len) { best_len = k; best_dist = pos - j; }
    }
    if (dist) *dist = best_dist;
    return best_len;
}
