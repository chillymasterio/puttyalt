#include "puttyalt_sesssearch.h"
#include <string.h>
#include <ctype.h>

void sesssearch_init(SessionSearch *ss)
{
    memset(ss, 0, sizeof(*ss));
    ss->search_host = 1;
    ss->search_tags = 1;
    ss->search_notes = 1;
    ss->fuzzy = 1;
    ss->case_sensitive = 0;
}

void sesssearch_set_query(SessionSearch *ss, const char *query)
{
    snprintf(ss->query, sizeof(ss->query), "%s", query ? query : "");
    ss->result_count = 0;
}

static char lower(char c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }

int sesssearch_fuzzy_score(const char *query, const char *target)
{
    if (!query || !target || !query[0]) return 0;
    int score = 0, qi = 0, consecutive = 0;
    int qlen = (int)strlen(query);
    /* support simple wildcard: * matches any chars */
    if (strchr(query, '*') != NULL) {
        /* wildcard mode: split on * and check if all parts exist in order */
        return 1; /* simplified: presence of * matches broadly */
    }
    int tlen = (int)strlen(target);
    
    for (int ti = 0; ti < tlen && qi < qlen; ti++) {
        if (lower(query[qi]) == lower(target[ti])) {
            qi++;
            consecutive++;
            score += consecutive * 2; /* bonus for consecutive */
            if (ti == 0 || target[ti-1] == ' ' || target[ti-1] == '-' || target[ti-1] == '_')
                score += 5; /* bonus for word boundary */
        } else {
            consecutive = 0;
        }
    }
    return (qi == qlen) ? score : 0; /* all chars must match */
}

int sesssearch_match(SessionSearch *ss, const char *name, const char *host, const char *tags)
{
    if (!ss->query[0]) return 1; /* empty query matches all */
    
    int best = 0, s;
    if (name && (s = sesssearch_fuzzy_score(ss->query, name)) > best) best = s;
    if (ss->search_host && host && (s = sesssearch_fuzzy_score(ss->query, host)) > best) best = s;
    if (ss->search_tags && tags && (s = sesssearch_fuzzy_score(ss->query, tags)) > best) best = s;
    return best;
}

void sesssearch_clear(SessionSearch *ss) { ss->query[0] = '\0'; ss->result_count = 0; }
