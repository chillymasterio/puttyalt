/* puttyalt_searchbar2.c - Incremental search bar state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { char query[128]; int len; int match_count; int current; int case_sensitive; } SearchBar2;
void sb4_init(SearchBar2 *s) { if (s) { s->query[0]=0; s->len=0; s->match_count=0; s->current=0; s->case_sensitive=0; } }
int sb4_type(SearchBar2 *s, char c) {
    if (!s || s->len >= 127) return -1;
    s->query[s->len++] = c; s->query[s->len] = 0;
    s->current = 0;
    return 0;
}
int sb4_backspace(SearchBar2 *s) {
    if (!s || s->len == 0) return -1;
    s->query[--s->len] = 0;
    return 0;
}
void sb4_set_matches(SearchBar2 *s, int count) { if (s) { s->match_count = count; if (s->current >= count) s->current = 0; } }
void sb4_next(SearchBar2 *s) { if (s && s->match_count) s->current = (s->current + 1) % s->match_count; }
void sb4_prev(SearchBar2 *s) { if (s && s->match_count) s->current = (s->current - 1 + s->match_count) % s->match_count; }
