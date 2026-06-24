/* puttyalt_historysearch.c - Search command history with filters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define HS_MAX 256
#define HS_LEN 128
typedef struct { char cmds[HS_MAX][HS_LEN]; int n; } HistorySearch;
void hs3_init(HistorySearch *h) { if (h) h->n = 0; }
int hs3_add(HistorySearch *h, const char *cmd) {
    if (!h || !cmd) return -1;
    if (h->n >= HS_MAX) {
        for (int i = 1; i < HS_MAX; i++) memcpy(h->cmds[i-1], h->cmds[i], HS_LEN);
        h->n = HS_MAX - 1;
    }
    strncpy(h->cmds[h->n], cmd, HS_LEN-1); h->cmds[h->n][HS_LEN-1] = 0;
    h->n++;
    return 0;
}
/* Find most recent command containing `substr`; returns index or -1. */
int hs3_rsearch(const HistorySearch *h, const char *substr) {
    if (!h || !substr) return -1;
    for (int i = h->n - 1; i >= 0; i--) if (strstr(h->cmds[i], substr)) return i;
    return -1;
}
int hs3_prefix_match(const HistorySearch *h, const char *prefix, int *out, int cap) {
    if (!h || !prefix) return 0;
    int pl = (int)strlen(prefix), c = 0;
    for (int i = h->n - 1; i >= 0 && c < cap; i--) if (strncmp(h->cmds[i], prefix, pl) == 0) out[c++] = i;
    return c;
}
