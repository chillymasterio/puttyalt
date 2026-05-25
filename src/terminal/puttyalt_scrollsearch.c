#include <string.h>
#include <stdio.h>

#define MAX_RESULTS 256

typedef struct { int line; int col; int len; } SearchHit;
typedef struct {
    char query[256];
    SearchHit hits[MAX_RESULTS];
    int hit_count;
    int current;
    int case_sensitive;
    int regex_mode;
} ScrollSearch;

void scrollsearch_init(ScrollSearch *s)
{
    memset(s, 0, sizeof(*s));
}

static int ci_strstr(const char *haystack, const char *needle)
{
    int hlen = (int)strlen(haystack), nlen = (int)strlen(needle);
    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            char a = haystack[i+j], b = needle[j];
            if (a >= 'A' && a <= 'Z') a += 32;
            if (b >= 'A' && b <= 'Z') b += 32;
            if (a != b) { match = 0; break; }
        }
        if (match) return i;
    }
    return -1;
}

int scrollsearch_execute(ScrollSearch *s, const char **lines, int line_count)
{
    s->hit_count = 0;
    if (!s->query[0]) return 0;
    for (int i = 0; i < line_count && s->hit_count < MAX_RESULTS; i++) {
        int col = s->case_sensitive ?
            (strstr(lines[i], s->query) ? (int)(strstr(lines[i], s->query) - lines[i]) : -1) :
            ci_strstr(lines[i], s->query);
        if (col >= 0) {
            s->hits[s->hit_count].line = i;
            s->hits[s->hit_count].col = col;
            s->hits[s->hit_count].len = (int)strlen(s->query);
            s->hit_count++;
        }
    }
    s->current = 0;
    return s->hit_count;
}

SearchHit *scrollsearch_next(ScrollSearch *s)
{
    if (s->hit_count == 0) return NULL;
    SearchHit *h = &s->hits[s->current];
    s->current = (s->current + 1) % s->hit_count;
    return h;
}

SearchHit *scrollsearch_prev(ScrollSearch *s)
{
    if (s->hit_count == 0) return NULL;
    s->current = (s->current - 1 + s->hit_count) % s->hit_count;
    return &s->hits[s->current];
}
