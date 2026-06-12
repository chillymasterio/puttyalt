/* puttyalt_sessbookmark.c - In-session scroll bookmarks with labels. */
#include <string.h>
#include <stdio.h>
#define SB_MAX 32
#define SB_LABEL 32
typedef struct {
    int line;
    char label[SB_LABEL];
    int auto_created;
} sb_mark;
typedef struct {
    sb_mark marks[SB_MAX];
    int n;
} SessBookmark;
void sessbookmark_init(SessBookmark *s) {
    if (s) memset(s, 0, sizeof(*s));
}
int sessbookmark_add(SessBookmark *s, int line, const char *label, int auto_created) {
    if (!s || s->n >= SB_MAX) return -1;
    sb_mark *m = &s->marks[s->n];
    m->line = line;
    snprintf(m->label, SB_LABEL, "%s", label ? label : "");
    m->auto_created = auto_created ? 1 : 0;
    return s->n++;
}
int sessbookmark_nearest(const SessBookmark *s, int line) {
    if (!s || s->n == 0) return -1;
    int best = 0, bestdist = 1 << 30;
    for (int i = 0; i < s->n; i++) {
        int d = s->marks[i].line - line;
        if (d < 0) d = -d;
        if (d < bestdist) {
            bestdist = d;
            best = i;
        }
    }
    return best;
}
int sessbookmark_remove(SessBookmark *s, int idx) {
    if (!s || idx < 0 || idx >= s->n) return -1;
    memmove(&s->marks[idx], &s->marks[idx + 1], sizeof(sb_mark) * (s->n - idx - 1));
    s->n--;
    return 0;
}
int sessbookmark_count(const SessBookmark *s) {
    return s ? s->n : -1;
}
