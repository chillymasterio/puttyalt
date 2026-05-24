#include "puttyalt_searchhl.h"
#include <string.h>
#include <stdio.h>

void searchhl_init(SearchHighlight *sh)
{
    memset(sh, 0, sizeof(*sh));
    sh->wrap_around = 1;
    sh->highlight_fg = 0x000000;
    sh->highlight_bg = 0xFFD740;
    sh->current_fg = 0x000000;
    sh->current_bg = 0xFF6D00;
}

int searchhl_search(SearchHighlight *sh, const char *query,
                    const char **lines, int num_lines)
{
    if (!query || query[0] == '\0') { searchhl_clear(sh); return 0; }

    snprintf(sh->query, sizeof(sh->query), "%s", query);
    sh->num_matches = 0;
    sh->current_match = 0;
    sh->active = 1;

    int qlen = (int)strlen(query);

    for (int row = 0; row < num_lines && sh->num_matches < SEARCH_MAX_RESULTS; row++) {
        if (!lines[row]) continue;
        const char *line = lines[row];
        int llen = (int)strlen(line);

        for (int col = 0; col <= llen - qlen; col++) {
            int match = 1;
            for (int k = 0; k < qlen; k++) {
                char a = line[col + k];
                char b = query[k];
                if (!sh->case_sensitive) {
                    if (a >= 'A' && a <= 'Z') a += 32;
                    if (b >= 'A' && b <= 'Z') b += 32;
                }
                if (a != b) { match = 0; break; }
            }
            if (match) {
                SearchMatch *m = &sh->matches[sh->num_matches++];
                m->row = row;
                m->col = col;
                m->length = qlen;
                if (sh->num_matches >= SEARCH_MAX_RESULTS) break;
            }
        }
    }

    return sh->num_matches;
}

int searchhl_next(SearchHighlight *sh)
{
    if (sh->num_matches == 0) return -1;
    sh->current_match = (sh->current_match + 1) % sh->num_matches;
    return sh->current_match;
}

int searchhl_prev(SearchHighlight *sh)
{
    if (sh->num_matches == 0) return -1;
    sh->current_match = (sh->current_match - 1 + sh->num_matches) % sh->num_matches;
    return sh->current_match;
}

int searchhl_get_current(const SearchHighlight *sh, int *row, int *col)
{
    if (sh->num_matches == 0) return -1;
    *row = sh->matches[sh->current_match].row;
    *col = sh->matches[sh->current_match].col;
    return sh->current_match;
}

void searchhl_clear(SearchHighlight *sh)
{
    sh->num_matches = 0;
    sh->current_match = 0;
    sh->query[0] = '\0';
    sh->active = 0;
}

int searchhl_is_highlighted(const SearchHighlight *sh, int row, int col)
{
    if (!sh->active) return 0;
    for (int i = 0; i < sh->num_matches; i++) {
        const SearchMatch *m = &sh->matches[i];
        if (m->row == row && col >= m->col && col < m->col + m->length)
            return (i == sh->current_match) ? 2 : 1;
    }
    return 0;
}
