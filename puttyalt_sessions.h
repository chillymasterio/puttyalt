/*
 * puttyalt_sessions.h: Session manager with folder organisation.
 *
 * Extends PuTTY's flat session list with folder hierarchy support,
 * tags, and quick-search. Sessions are stored as JSON files in
 * the config directory.
 */

#ifndef PUTTYALT_SESSIONS_H
#define PUTTYALT_SESSIONS_H

#define MAX_SESSION_NAME   256
#define MAX_SESSION_FOLDER 256
#define MAX_SESSION_TAG    64
#define MAX_TAGS_PER_SESSION 8

typedef struct SessionEntry {
    char name[MAX_SESSION_NAME];
    char folder[MAX_SESSION_FOLDER]; /* e.g. "Production/Web" */
    char hostname[256];
    int port;
    char username[64];
    char tags[MAX_TAGS_PER_SESSION][MAX_SESSION_TAG];
    int tag_count;

    /* Metadata */
    long last_used;       /* unix timestamp */
    int use_count;
    int favourite;        /* pinned to top */

    struct SessionEntry *next;
} SessionEntry;

typedef struct SessionManager {
    SessionEntry *sessions;
    int count;
    char config_dir[4096];
} SessionManager;

/* Initialise session manager, loading saved sessions */
int session_mgr_init(SessionManager *mgr, const char *config_dir);

/* Free all loaded sessions */
void session_mgr_free(SessionManager *mgr);

/* Add a new session */
int session_mgr_add(SessionManager *mgr, const SessionEntry *entry);

/* Remove a session by name */
int session_mgr_remove(SessionManager *mgr, const char *name);

/* Find a session by name */
SessionEntry *session_mgr_find(SessionManager *mgr, const char *name);

/* Find sessions matching a search query (name, hostname, or tag) */
int session_mgr_search(SessionManager *mgr, const char *query,
                       SessionEntry **results, int max_results);

/* List sessions in a folder */
int session_mgr_list_folder(SessionManager *mgr, const char *folder,
                            SessionEntry **results, int max_results);

/* Save all sessions to disk */
int session_mgr_save(const SessionManager *mgr);

/* Record that a session was used (updates last_used, use_count) */
void session_mgr_touch(SessionEntry *entry);

#endif /* PUTTYALT_SESSIONS_H */
