/* puttyalt_sesshistory.c - Session command history with dedup + search. */
#include <string.h>
#include <stdio.h>
#define SH_MAX 256
#define SH_CMD 256
typedef struct {
    char commands[SH_MAX][SH_CMD];
    int n;
    int cursor;
} SessHistory;
void sesshistory_init(SessHistory *s) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
    s->cursor = -1;
}
int sesshistory_add(SessHistory *s, const char *cmd) {
    if (!s || !cmd || !*cmd) return -1;
    if (s->n > 0 && strcmp(s->commands[s->n - 1], cmd) == 0) return 0;
    if (s->n >= SH_MAX) {
        memmove(s->commands[0], s->commands[1], sizeof(s->commands[0]) * (SH_MAX - 1));
        s->n--;
    }
    snprintf(s->commands[s->n++], SH_CMD, "%s", cmd);
    s->cursor = s->n;
    return 0;
}
const char *sesshistory_prev(SessHistory *s) {
    if (!s || s->n == 0) return NULL;
    if (s->cursor > 0) s->cursor--;
    return s->commands[s->cursor];
}
const char *sesshistory_next(SessHistory *s) {
    if (!s || s->cursor >= s->n - 1) {
        if (s) s->cursor = s->n;
        return "";
    }
    s->cursor++;
    return s->commands[s->cursor];
}
int sesshistory_search(const SessHistory *s, const char *query, int *out_idx, int cap) {
    if (!s || !query) return -1;
    int n = 0;
    for (int i = s->n - 1; i >= 0 && n < cap; i--) {
        if (strstr(s->commands[i], query)) out_idx[n++] = i;
    }
    return n;
}
int sesshistory_count(const SessHistory *s) {
    return s ? s->n : -1;
}
