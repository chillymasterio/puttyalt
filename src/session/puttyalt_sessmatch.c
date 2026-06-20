/* puttyalt_sessmatch.c - Fuzzy-match sessions for quick switch.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
/* Score how well `query` matches `name` (subsequence). 0 = no match. */
int smt_score(const char *query, const char *name) {
    if (!query || !name) return 0;
    if (!*query) return 1;
    int score = 0, consec = 0;
    const char *n = name;
    for (const char *q = query; *q; q++) {
        int found = 0;
        while (*n) {
            if (tolower((unsigned char)*n) == tolower((unsigned char)*q)) { found = 1; n++; break; }
            n++; consec = 0;
        }
        if (!found) return 0;
        score += 1 + consec;
        consec++;
    }
    return score;
}
/* Pick best-scoring index from an array of names. */
int smt_best(const char *query, const char *const *names, int count) {
    int best = -1, best_score = 0;
    for (int i = 0; i < count; i++) {
        int s = smt_score(query, names[i]);
        if (s > best_score) { best_score = s; best = i; }
    }
    return best;
}
