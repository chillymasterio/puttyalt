#ifndef PUTTYALT_SESSSEARCH_H
#define PUTTYALT_SESSSEARCH_H

#define SEARCH_MAX_RESULTS 64

typedef struct {
    char query[256];
    int  results[SEARCH_MAX_RESULTS];
    int  result_count;
    int  search_host;
    int  search_tags;
    int  search_notes;
    int  fuzzy;
    int  case_sensitive;
} SessionSearch;

void sesssearch_init(SessionSearch *ss);
void sesssearch_set_query(SessionSearch *ss, const char *query);
int  sesssearch_fuzzy_score(const char *query, const char *target);
int  sesssearch_match(SessionSearch *ss, const char *name, const char *host, const char *tags);
void sesssearch_clear(SessionSearch *ss);

#endif
