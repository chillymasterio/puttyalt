/* puttyalt_sessiontag2.c - Tag sessions with colored labels.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define ST_MAX 64
typedef struct { int session_id; char tag[24]; int color; } StTag;
typedef struct { StTag tags[ST_MAX]; int n; } SessionTags2;
void st3_init(SessionTags2 *s) { if (s) s->n = 0; }
int st3_tag(SessionTags2 *s, int session_id, const char *tag, int color) {
    if (!s || s->n >= ST_MAX || !tag) return -1;
    s->tags[s->n].session_id = session_id;
    strncpy(s->tags[s->n].tag, tag, 23); s->tags[s->n].tag[23] = 0;
    s->tags[s->n].color = color;
    s->n++;
    return 0;
}
int st3_find_by_tag(const SessionTags2 *s, const char *tag, int *out, int cap) {
    if (!s || !tag) return 0;
    int c = 0;
    for (int i = 0; i < s->n && c < cap; i++) if (strcmp(s->tags[i].tag, tag) == 0) out[c++] = s->tags[i].session_id;
    return c;
}
