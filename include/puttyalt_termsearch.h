/*
 * puttyalt_termsearch.h: In-terminal text search (Ctrl+Shift+F).
 *
 * Search the scrollback buffer for a string or pattern.
 * Results are highlighted in the terminal and the user can
 * navigate between matches with Ctrl+Shift+N / Ctrl+Shift+P.
 */

#ifndef PUTTYALT_TERMSEARCH_H
#define PUTTYALT_TERMSEARCH_H

#define MAX_SEARCH_QUERY  256
#define MAX_SEARCH_HITS   4096

typedef struct SearchHit {
    int   line;     /* scrollback line number (0 = current) */
    int   col;      /* column offset */
    int   len;      /* length of match */
} SearchHit;

typedef struct TermSearch {
    char       query[MAX_SEARCH_QUERY];
    int        case_sensitive;
    int        wrap_around;

    SearchHit  hits[MAX_SEARCH_HITS];
    int        hit_count;
    int        current;     /* index of currently-selected hit */

    int        active;      /* search bar is open */
} TermSearch;

/* Initialise search state */
void termsearch_init(TermSearch *ts);

/* Start a new search.  Clears previous results. */
void termsearch_begin(TermSearch *ts, const char *query, int case_sensitive);

/* Add a hit (called by the terminal scan loop) */
int termsearch_add_hit(TermSearch *ts, int line, int col, int len);

/* Navigate to next / previous hit.  Returns the hit index. */
int termsearch_next(TermSearch *ts);
int termsearch_prev(TermSearch *ts);

/* Get the currently selected hit, or NULL */
const SearchHit *termsearch_current(const TermSearch *ts);

/* Close search, clear highlights */
void termsearch_close(TermSearch *ts);

#endif /* PUTTYALT_TERMSEARCH_H */
