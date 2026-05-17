#ifndef PUTTYALT_CMDHIST_V2_H
#define PUTTYALT_CMDHIST_V2_H

#define HIST_MAX 4096

typedef struct {
    char command[512];
    char session[128];
    char host[256];
    unsigned long timestamp;
    int  exit_code;
    int  duration_ms;
} HistEntry;

typedef struct {
    HistEntry *entries;
    int count;
    int capacity;
    int deduplicate;
    int max_entries;
    char db_path[512];
} CommandHistBrowser;

void cmdhist_init(CommandHistBrowser *ch);
void cmdhist_free(CommandHistBrowser *ch);
int  cmdhist_add(CommandHistBrowser *ch, const char *cmd, const char *session, const char *host);
int  cmdhist_search(CommandHistBrowser *ch, const char *query, int *results, int max);
int  cmdhist_search_session(CommandHistBrowser *ch, const char *session, int *results, int max);
int  cmdhist_save(CommandHistBrowser *ch, const char *path);
int  cmdhist_load(CommandHistBrowser *ch, const char *path);
void cmdhist_clear_session(CommandHistBrowser *ch, const char *session);
void cmdhist_clear_all(CommandHistBrowser *ch);
HistEntry *cmdhist_get(CommandHistBrowser *ch, int index);

#endif
