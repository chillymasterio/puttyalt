#ifndef PUTTYALT_SEARCHOVERLAY_H
#define PUTTYALT_SEARCHOVERLAY_H

#define SEARCH_MAX_MATCHES 1024

typedef struct {
    int line;
    int col;
    int length;
} SearchMatch;

typedef struct {
    char query[256];
    int  active;
    int  case_sensitive;
    int  regex_mode;
    int  wrap_around;
    int  highlight_all;
    SearchMatch matches[SEARCH_MAX_MATCHES];
    int match_count;
    int current_match;
    int search_direction; /* 0=forward, 1=backward */
} SearchOverlay;

void searchov_init(SearchOverlay *so);
void searchov_open(SearchOverlay *so);
void searchov_close(SearchOverlay *so);
void searchov_set_query(SearchOverlay *so, const char *query);
int  searchov_find_in_buffer(SearchOverlay *so, const char **lines, int line_count);
void searchov_next(SearchOverlay *so);
void searchov_prev(SearchOverlay *so);
SearchMatch *searchov_current(SearchOverlay *so);
void searchov_toggle_case(SearchOverlay *so);
void searchov_toggle_regex(SearchOverlay *so);

#endif
