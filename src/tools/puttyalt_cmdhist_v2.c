#include <stdio.h>
#include "puttyalt_cmdhist_v2.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void cmdhist_init(CommandHistBrowser *ch)
{
    memset(ch, 0, sizeof(*ch));
    ch->capacity = 1024;
    ch->max_entries = HIST_MAX;
    ch->deduplicate = 1;
    ch->entries = (HistEntry *)calloc(ch->capacity, sizeof(HistEntry));
}

void cmdhist_free(CommandHistBrowser *ch) { free(ch->entries); memset(ch, 0, sizeof(*ch)); }

int cmdhist_add(CommandHistBrowser *ch, const char *cmd, const char *session, const char *host)
{
    if (!cmd || !cmd[0]) return -1;
    /* dedup: skip if last command is identical */
    if (ch->deduplicate && ch->count > 0 &&
        strcmp(ch->entries[ch->count-1].command, cmd) == 0 &&
        strcmp(ch->entries[ch->count-1].session, session ? session : "") == 0)
        return 0;
    if (ch->count >= ch->capacity) {
        if (ch->capacity >= ch->max_entries) {
            /* evict oldest 25% */
            int evict = ch->count / 4;
            memmove(ch->entries, ch->entries + evict, (ch->count - evict) * sizeof(HistEntry));
            ch->count -= evict;
        } else {
            ch->capacity *= 2;
            if (ch->capacity > ch->max_entries) ch->capacity = ch->max_entries;
            ch->entries = (HistEntry *)realloc(ch->entries, ch->capacity * sizeof(HistEntry));
        }
    }
    HistEntry *e = &ch->entries[ch->count++];
    memset(e, 0, sizeof(*e));
    snprintf(e->command, sizeof(e->command), "%s", cmd);
    snprintf(e->session, sizeof(e->session), "%s", session ? session : "");
    snprintf(e->host, sizeof(e->host), "%s", host ? host : "");
    e->timestamp = (unsigned long)time(NULL);
    return 0;
}

int cmdhist_search(CommandHistBrowser *ch, const char *query, int *results, int max)
{
    int n = 0;
    for (int i = ch->count - 1; i >= 0 && n < max; i--)
        if (strstr(ch->entries[i].command, query)) results[n++] = i;
    return n;
}

int cmdhist_search_session(CommandHistBrowser *ch, const char *session, int *results, int max)
{
    int n = 0;
    for (int i = ch->count - 1; i >= 0 && n < max; i--)
        if (strcmp(ch->entries[i].session, session) == 0) results[n++] = i;
    return n;
}

int cmdhist_save(CommandHistBrowser *ch, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ch->count; i++) {
        HistEntry *e = &ch->entries[i];
        fprintf(f, "%lu\t%s\t%s\t%s\n", e->timestamp, e->session, e->host, e->command);
    }
    fclose(f);
    return 0;
}

int cmdhist_load(CommandHistBrowser *ch, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        unsigned long ts;
        char sess[128], host[256], cmd[512];
        if (sscanf(line, "%lu\t%127[^\t]\t%255[^\t]\t%511[^\n]", &ts, sess, host, cmd) >= 4) {
            cmdhist_add(ch, cmd, sess, host);
            if (ch->count > 0) ch->entries[ch->count-1].timestamp = ts;
        }
    }
    fclose(f);
    return 0;
}

void cmdhist_clear_session(CommandHistBrowser *ch, const char *session)
{
    int w = 0;
    for (int r = 0; r < ch->count; r++)
        if (strcmp(ch->entries[r].session, session) != 0)
            ch->entries[w++] = ch->entries[r];
    ch->count = w;
}

void cmdhist_clear_all(CommandHistBrowser *ch) { ch->count = 0; }
HistEntry *cmdhist_get(CommandHistBrowser *ch, int index) { return (index >= 0 && index < ch->count) ? &ch->entries[index] : NULL; }
