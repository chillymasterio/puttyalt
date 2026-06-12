/* puttyalt_logsearch.c - Indexed log search with ring-buffered lines + term match. */
#include <string.h>
#include <stdio.h>
#define LS_LINES 512
#define LS_LINE 256
typedef struct { char lines[LS_LINES][LS_LINE]; int head, count; } LogSearch;
void logsearch_init(LogSearch *l) { if(l) memset(l,0,sizeof(*l)); }
void logsearch_append(LogSearch *l, const char *line) {
    if(!l||!line) return;
    int idx=(l->head + l->count)%LS_LINES;
    if (l->count<LS_LINES) l->count++; else l->head=(l->head+1)%LS_LINES;
    snprintf(l->lines[idx],LS_LINE,"%s",line);
}
int logsearch_find(const LogSearch *l, const char *term, int *out_indices, int cap) {
    if(!l||!term) return -1;
    int found=0;
    for (int i=0;i<l->count && found<cap;i++) {
        int idx=(l->head+i)%LS_LINES;
        if (strstr(l->lines[idx],term)) { if(out_indices) out_indices[found]=i; found++; }
    }
    return found;
}
const char *logsearch_line(const LogSearch *l, int i) {
    if(!l||i<0||i>=l->count) return NULL;
    return l->lines[(l->head+i)%LS_LINES];
}
int logsearch_count(const LogSearch *l) { return l?l->count:-1; }
