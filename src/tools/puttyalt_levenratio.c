/* puttyalt_levenratio.c - Similarity ratio from edit distance.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static int lvr_min3(int a, int b, int c) { int m = a < b ? a : b; return m < c ? m : c; }
int lvr_distance(const char *a, const char *b) {
    int la = (int)strlen(a), lb = (int)strlen(b);
    if (la > 128) la = 128;
    if (lb > 128) lb = 128;
    int prev[129], cur[129];
    for (int j = 0; j <= lb; j++) prev[j] = j;
    for (int i = 1; i <= la; i++) {
        cur[0] = i;
        for (int j = 1; j <= lb; j++) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            cur[j] = lvr_min3(prev[j] + 1, cur[j-1] + 1, prev[j-1] + cost);
        }
        for (int j = 0; j <= lb; j++) prev[j] = cur[j];
    }
    return prev[lb];
}
/* Ratio in 0..100 (100 == identical). */
int lvr_ratio(const char *a, const char *b) {
    int la = (int)strlen(a), lb = (int)strlen(b);
    int max = la > lb ? la : lb;
    if (max == 0) return 100;
    int d = lvr_distance(a, b);
    return (int)(100 - (d * 100) / max);
}
