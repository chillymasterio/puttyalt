#include <string.h>
#include <ctype.h>
#include "puttyalt_findreplace.h"

void fr_init(FindReplace *fr)
{
    memset(fr, 0, sizeof(*fr));
    fr->current_match = -1;
    fr->case_sensitive = 0;
    fr->wrap_around = 1;
    fr->highlight_all = 1;
}

static int ci_strstr(const char *haystack, const char *needle, int *match_len)
{
    int nlen = strlen(needle);
    int hlen = strlen(haystack);
    *match_len = nlen;
    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i+j]) !=
                tolower((unsigned char)needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

int fr_search(FindReplace *fr, const char *text, int text_len,
              const char *pattern)
{
    fr->num_matches = 0;
    fr->current_match = -1;
    strncpy(fr->pattern, pattern, FR_MAX_PATTERN - 1);

    int line = 0, col = 0;
    for (int i = 0; i < text_len && fr->num_matches < FR_MAX_MATCHES; i++) {
        if (text[i] == '\n') { line++; col = 0; continue; }

        const char *pos = &text[i];
        int remaining = text_len - i;
        int plen = strlen(pattern);
        if (remaining < plen) break;

        int matched = 0;
        if (fr->case_sensitive) {
            if (strncmp(pos, pattern, plen) == 0) matched = 1;
        } else {
            int ml;
            if (ci_strstr(pos, pattern, &ml) == 0) matched = 1;
        }

        if (matched) {
            FindMatch *m = &fr->matches[fr->num_matches];
            m->line = line;
            m->col = col;
            m->len = plen;
            fr->num_matches++;
            i += plen - 1;
            col += plen;
        } else {
            col++;
        }
    }

    if (fr->num_matches > 0) fr->current_match = 0;
    return fr->num_matches;
}

int fr_next(FindReplace *fr)
{
    if (fr->num_matches == 0) return -1;
    fr->current_match++;
    if (fr->current_match >= fr->num_matches) {
        if (fr->wrap_around) fr->current_match = 0;
        else { fr->current_match--; return -1; }
    }
    return fr->current_match;
}

int fr_prev(FindReplace *fr)
{
    if (fr->num_matches == 0) return -1;
    fr->current_match--;
    if (fr->current_match < 0) {
        if (fr->wrap_around) fr->current_match = fr->num_matches - 1;
        else { fr->current_match = 0; return -1; }
    }
    return fr->current_match;
}

void fr_clear(FindReplace *fr)
{
    fr->num_matches = 0;
    fr->current_match = -1;
    fr->pattern[0] = '\0';
}

int fr_get_current_line(const FindReplace *fr)
{
    if (fr->current_match < 0) return -1;
    return fr->matches[fr->current_match].line;
}

int fr_get_current_col(const FindReplace *fr)
{
    if (fr->current_match < 0) return -1;
    return fr->matches[fr->current_match].col;
}
