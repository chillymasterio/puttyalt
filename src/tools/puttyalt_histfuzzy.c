/* puttyalt_histfuzzy.c - fuzzy command history index for PuttyAlt.
 * Builds a lightweight in-memory fuzzy index over command history,
 * supporting subsequence matching with gap-penalized scoring.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HISTFZ_MAX_ENTRIES 512
#define HISTFZ_MAX_LEN     256

typedef struct HistFuzzyIndex {
    char  entries[HISTFZ_MAX_ENTRIES][HISTFZ_MAX_LEN];
    int   count;
} HistFuzzyIndex;

typedef struct HistFuzzyHit {
    int  index;                    /* slot in the index */
    int  score;                    /* match score, higher is better */
    char text[HISTFZ_MAX_LEN];     /* copy of the matched entry */
} HistFuzzyHit;

void histfz_init(HistFuzzyIndex *ix)
{
    if (!ix)
        return;
    ix->count = 0;
    memset(ix->entries, 0, sizeof(ix->entries));
}

int histfz_add(HistFuzzyIndex *ix, const char *entry)
{
    size_t len;
    if (!ix || !entry)
        return -1;
    if (ix->count >= HISTFZ_MAX_ENTRIES)
        return -1;
    len = strlen(entry);
    if (len >= HISTFZ_MAX_LEN)
        return -1;
    memcpy(ix->entries[ix->count], entry, len);
    ix->entries[ix->count][len] = '\0';
    ix->count++;
    return 0;
}

/* Lowercase a single ASCII char without locale dependence. */
static int histfz_lc(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

/* Returns a score >= 0 if every char of needle appears in order within hay,
 * otherwise -1. Adjacent matches are rewarded; gaps are penalized. */
int histfz_subseq_score(const char *needle, const char *hay)
{
    int score = 0;
    int prev_match = -1;
    size_t hi = 0;
    const char *np;
    if (!needle || !hay)
        return -1;
    if (needle[0] == '\0')
        return 0;
    np = needle;
    while (*np != '\0') {
        int found = 0;
        for (; hay[hi] != '\0'; hi++) {
            if (histfz_lc((unsigned char)hay[hi]) ==
                histfz_lc((unsigned char)*np)) {
                int gap = (prev_match < 0) ? 0 : ((int)hi - prev_match - 1);
                score += 10;
                if (gap == 0 && prev_match >= 0)
                    score += 5;          /* contiguous bonus */
                else
                    score -= (gap > 8 ? 8 : gap);   /* clamped gap penalty */
                if (hi == 0)
                    score += 3;          /* prefix bonus */
                prev_match = (int)hi;
                hi++;
                found = 1;
                break;
            }
        }
        if (!found)
            return -1;
        np++;
    }
    return score < 0 ? 0 : score;
}

/* Fills out[] with up to outcap hits, sorted by descending score. */
int histfz_query(HistFuzzyIndex *ix, const char *needle,
                 HistFuzzyHit *out, int outcap)
{
    int i, n = 0;
    if (!ix || !needle || !out || outcap <= 0)
        return -1;
    for (i = 0; i < ix->count; i++) {
        int sc = histfz_subseq_score(needle, ix->entries[i]);
        if (sc < 0)
            continue;
        if (n < outcap) {
            out[n].index = i;
            out[n].score = sc;
            snprintf(out[n].text, HISTFZ_MAX_LEN, "%s", ix->entries[i]);
            n++;
        } else {
            int j, worst = 0;
            for (j = 1; j < n; j++)
                if (out[j].score < out[worst].score)
                    worst = j;
            if (sc > out[worst].score) {
                out[worst].index = i;
                out[worst].score = sc;
                snprintf(out[worst].text, HISTFZ_MAX_LEN, "%s",
                         ix->entries[i]);
            }
        }
    }
    /* insertion sort: descending score */
    for (i = 1; i < n; i++) {
        HistFuzzyHit tmp = out[i];
        int j = i - 1;
        while (j >= 0 && out[j].score < tmp.score) {
            out[j + 1] = out[j];
            j--;
        }
        out[j + 1] = tmp;
    }
    return n;
}

void histfz_clear(HistFuzzyIndex *ix)
{
    if (!ix)
        return;
    ix->count = 0;
    memset(ix->entries, 0, sizeof(ix->entries));
}
