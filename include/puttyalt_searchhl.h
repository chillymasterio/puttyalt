#ifndef PUTTYALT_SEARCHHL_H
#define PUTTYALT_SEARCHHL_H

#define SEARCH_MAX_RESULTS 1024
#define SEARCH_MAX_QUERY   256

typedef struct {
    int row;
    int col;
    int length;
} SearchMatch;

typedef struct {
    char query[SEARCH_MAX_QUERY];
    SearchMatch matches[SEARCH_MAX_RESULTS];
    int num_matches;
    int current_match;
    int case_sensitive;
    int regex_mode;
    int wrap_around;
    int active;
    unsigned int highlight_fg;
    unsigned int highlight_bg;
    unsigned int current_fg;
    unsigned int current_bg;
} SearchHighlight;

void searchhl_init(SearchHighlight *sh);
int  searchhl_search(SearchHighlight *sh, const char *query,
                     const char **lines, int num_lines);
int  searchhl_next(SearchHighlight *sh);
int  searchhl_prev(SearchHighlight *sh);
int  searchhl_get_current(const SearchHighlight *sh, int *row, int *col);
void searchhl_clear(SearchHighlight *sh);
int  searchhl_is_highlighted(const SearchHighlight *sh, int row, int col);

#endif
