/*
 * puttyalt_sessions.c: Session manager with folder organisation.
 */

#include "puttyalt_sessions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int session_mgr_init(SessionManager *mgr, const char *config_dir)
{
    memset(mgr, 0, sizeof(*mgr));
    snprintf(mgr->config_dir, sizeof(mgr->config_dir), "%s", config_dir);
    mgr->sessions = NULL;
    mgr->count = 0;
    return 0;
}

void session_mgr_free(SessionManager *mgr)
{
    SessionEntry *e = mgr->sessions;
    while (e) {
        SessionEntry *next = e->next;
        free(e);
        e = next;
    }
    mgr->sessions = NULL;
    mgr->count = 0;
}

int session_mgr_add(SessionManager *mgr, const SessionEntry *entry)
{
    /* Check for duplicate */
    if (session_mgr_find(mgr, entry->name))
        return -1;

    SessionEntry *e = malloc(sizeof(SessionEntry));
    if (!e) return -1;

    memcpy(e, entry, sizeof(SessionEntry));
    e->next = mgr->sessions;
    mgr->sessions = e;
    mgr->count++;
    return 0;
}

int session_mgr_remove(SessionManager *mgr, const char *name)
{
    SessionEntry **pp = &mgr->sessions;
    while (*pp) {
        if (strcmp((*pp)->name, name) == 0) {
            SessionEntry *doomed = *pp;
            *pp = doomed->next;
            free(doomed);
            mgr->count--;
            return 0;
        }
        pp = &(*pp)->next;
    }
    return -1;
}

SessionEntry *session_mgr_find(SessionManager *mgr, const char *name)
{
    SessionEntry *e;
    for (e = mgr->sessions; e; e = e->next) {
        if (strcmp(e->name, name) == 0)
            return e;
    }
    return NULL;
}

/*
 * Simple substring match for search — matches name, hostname,
 * folder, or any tag.
 */
static int entry_matches(const SessionEntry *e, const char *query)
{
    if (strstr(e->name, query)) return 1;
    if (strstr(e->hostname, query)) return 1;
    if (strstr(e->folder, query)) return 1;
    for (int i = 0; i < e->tag_count; i++) {
        if (strstr(e->tags[i], query)) return 1;
    }
    return 0;
}

int session_mgr_search(SessionManager *mgr, const char *query,
                       SessionEntry **results, int max_results)
{
    int n = 0;
    SessionEntry *e;
    for (e = mgr->sessions; e && n < max_results; e = e->next) {
        if (entry_matches(e, query))
            results[n++] = e;
    }
    return n;
}

int session_mgr_list_folder(SessionManager *mgr, const char *folder,
                            SessionEntry **results, int max_results)
{
    int n = 0;
    SessionEntry *e;
    for (e = mgr->sessions; e && n < max_results; e = e->next) {
        if (strcmp(e->folder, folder) == 0)
            results[n++] = e;
    }
    return n;
}

void session_mgr_touch(SessionEntry *entry)
{
    entry->last_used = (long)time(NULL);
    entry->use_count++;
}

int session_mgr_save(const SessionManager *mgr)
{
    char path[4096];
    snprintf(path, sizeof(path), "%s/sessions.dat", mgr->config_dir);

    FILE *f = fopen(path, "w");
    if (!f) return -1;

    SessionEntry *e;
    for (e = mgr->sessions; e; e = e->next) {
        fprintf(f, "[session]\n");
        fprintf(f, "name=%s\n", e->name);
        fprintf(f, "folder=%s\n", e->folder);
        fprintf(f, "hostname=%s\n", e->hostname);
        fprintf(f, "port=%d\n", e->port);
        fprintf(f, "username=%s\n", e->username);
        fprintf(f, "last_used=%ld\n", e->last_used);
        fprintf(f, "use_count=%d\n", e->use_count);
        fprintf(f, "favourite=%d\n", e->favourite);
        for (int i = 0; i < e->tag_count; i++)
            fprintf(f, "tag=%s\n", e->tags[i]);
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}
