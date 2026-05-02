/*
 * puttyalt_termsearch.c: In-terminal text search implementation.
 */

#include "puttyalt_termsearch.h"
#include <string.h>
#include <ctype.h>

void termsearch_init(TermSearch *ts)
{
    memset(ts, 0, sizeof(*ts));
    ts->wrap_around = 1;    /* wrap by default */
    ts->current = -1;
}

void termsearch_begin(TermSearch *ts, const char *query, int case_sensitive)
{
    ts->hit_count = 0;
    ts->current = -1;
    ts->active = 1;
    ts->case_sensitive = case_sensitive;

    if (query)
        snprintf(ts->query, sizeof(ts->query), "%s", query);
    else
        ts->query[0] = '\0';
}

int termsearch_add_hit(TermSearch *ts, int line, int col, int len)
{
    if (ts->hit_count >= MAX_SEARCH_HITS)
        return -1;

    SearchHit *h = &ts->hits[ts->hit_count];
    h->line = line;
    h->col = col;
    h->len = len;

    return ts->hit_count++;
}

int termsearch_next(TermSearch *ts)
{
    if (ts->hit_count == 0)
        return -1;

    ts->current++;
    if (ts->current >= ts->hit_count) {
        if (ts->wrap_around)
            ts->current = 0;
        else
            ts->current = ts->hit_count - 1;
    }

    return ts->current;
}

int termsearch_prev(TermSearch *ts)
{
    if (ts->hit_count == 0)
        return -1;

    ts->current--;
    if (ts->current < 0) {
        if (ts->wrap_around)
            ts->current = ts->hit_count - 1;
        else
            ts->current = 0;
    }

    return ts->current;
}

const SearchHit *termsearch_current(const TermSearch *ts)
{
    if (ts->current < 0 || ts->current >= ts->hit_count)
        return NULL;
    return &ts->hits[ts->current];
}

void termsearch_close(TermSearch *ts)
{
    ts->active = 0;
    ts->hit_count = 0;
    ts->current = -1;
    ts->query[0] = '\0';
}
