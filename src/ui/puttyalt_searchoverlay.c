#include <stdio.h>
#include "puttyalt_searchoverlay.h"
#include <string.h>
#include <ctype.h>

void searchov_init(SearchOverlay *so)
{
    memset(so, 0, sizeof(*so));
    so->wrap_around = 1;
    so->highlight_all = 1;
}

void searchov_open(SearchOverlay *so) { so->active = 1; }
void searchov_close(SearchOverlay *so) { so->active = 0; so->match_count = 0; so->current_match = 0; }

void searchov_set_query(SearchOverlay *so, const char *query)
{
    snprintf(so->query, sizeof(so->query), "%s", query ? query : "");
    so->match_count = 0;
    so->current_match = 0;
}

static int ci_strstr(const char *haystack, const char *needle, int *col)
{
    int nlen = (int)strlen(needle);
    int hlen = (int)strlen(haystack);
    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i+j]) != tolower((unsigned char)needle[j])) {
                match = 0; break;
            }
        }
        if (match) { *col = i; return 1; }
    }
    return 0;
}

int searchov_find_in_buffer(SearchOverlay *so, const char **lines, int line_count)
{
    if (!so->query[0]) return 0;
    so->match_count = 0;
    int qlen = (int)strlen(so->query);

    for (int i = 0; i < line_count && so->match_count < SEARCH_MAX_MATCHES; i++) {
        if (!lines[i]) continue;
        const char *line = lines[i];
        int pos = 0, hlen = (int)strlen(line);
        while (pos <= hlen - qlen) {
            int col;
            if (so->case_sensitive) {
                const char *found = strstr(line + pos, so->query);
                if (!found) break;
                col = (int)(found - line);
            } else {
                if (!ci_strstr(line + pos, so->query, &col)) break;
                col += pos;
            }
            SearchMatch *m = &so->matches[so->match_count++];
            m->line = i;
            m->col = col;
            m->length = qlen;
            pos = col + 1;
            if (so->match_count >= SEARCH_MAX_MATCHES) break;
        }
    }
    return so->match_count;
}

void searchov_next(SearchOverlay *so)
{
    if (so->match_count == 0) return;
    so->current_match = (so->current_match + 1) % so->match_count;
}

void searchov_prev(SearchOverlay *so)
{
    if (so->match_count == 0) return;
    so->current_match = (so->current_match - 1 + so->match_count) % so->match_count;
}

SearchMatch *searchov_current(SearchOverlay *so)
{
    return (so->match_count > 0) ? &so->matches[so->current_match] : NULL;
}

void searchov_toggle_case(SearchOverlay *so) { so->case_sensitive = !so->case_sensitive; }
void searchov_toggle_regex(SearchOverlay *so) { so->regex_mode = !so->regex_mode; }
