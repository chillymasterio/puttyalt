/* puttyalt_wsmanager.c - Workspace manager for PuttyAlt.
 * Manages named workspaces grouping sessions, with an active-workspace
 * pointer and add/remove/switch operations. Self-contained library module.
 */
#include <string.h>
#include <stdio.h>

#define WS_MAX_WORKSPACES   16
#define WS_MAX_SESSIONS     32
#define WS_NAME_LEN         64
#define WS_SESSION_LEN      64

typedef struct {
    int  id;                                        /* >=0 if in use, -1 free */
    char name[WS_NAME_LEN];
    char sessions[WS_MAX_SESSIONS][WS_SESSION_LEN];
    int  session_count;
} Workspace;

typedef struct {
    Workspace workspaces[WS_MAX_WORKSPACES];
    int       count;                                /* number of live slots */
    int       next_id;                              /* monotonic id source */
    int       active_id;                            /* -1 when none active */
} WorkspaceManager;

/* Locate the array index holding workspace ws_id, or -1 if not found. */
static int wsmgr_find(const WorkspaceManager *wm, int ws_id) {
    int i;
    if (wm == NULL || ws_id < 0) return -1;
    for (i = 0; i < WS_MAX_WORKSPACES; i++) {
        if (wm->workspaces[i].id == ws_id) return i;
    }
    return -1;
}

void wsmgr_init(WorkspaceManager *wm) {
    int i;
    if (wm == NULL) return;
    memset(wm, 0, sizeof(*wm));
    for (i = 0; i < WS_MAX_WORKSPACES; i++) {
        wm->workspaces[i].id = -1;
        wm->workspaces[i].session_count = 0;
    }
    wm->count = 0;
    wm->next_id = 1;
    wm->active_id = -1;
}

/* Create a named workspace; returns the new workspace id, or -1 on error. */
int wsmgr_create(WorkspaceManager *wm, const char *name) {
    int i, id;
    if (wm == NULL || name == NULL || name[0] == '\0') return -1;
    if (wm->count >= WS_MAX_WORKSPACES) return -1;
    for (i = 0; i < WS_MAX_WORKSPACES; i++) {
        if (wm->workspaces[i].id < 0) break;
    }
    if (i >= WS_MAX_WORKSPACES) return -1;
    id = wm->next_id++;
    wm->workspaces[i].id = id;
    snprintf(wm->workspaces[i].name, WS_NAME_LEN, "%s", name);
    wm->workspaces[i].session_count = 0;
    wm->count++;
    if (wm->active_id < 0) wm->active_id = id;
    return id;
}

int wsmgr_add_session(WorkspaceManager *wm, int ws_id, const char *session_name) {
    int idx;
    Workspace *w;
    if (wm == NULL || session_name == NULL || session_name[0] == '\0') return -1;
    idx = wsmgr_find(wm, ws_id);
    if (idx < 0) return -1;
    w = &wm->workspaces[idx];
    if (w->session_count >= WS_MAX_SESSIONS) return -1;
    snprintf(w->sessions[w->session_count], WS_SESSION_LEN, "%s", session_name);
    w->session_count++;
    return 0;
}

int wsmgr_switch(WorkspaceManager *wm, int ws_id) {
    if (wsmgr_find(wm, ws_id) < 0) return -1;
    wm->active_id = ws_id;
    return 0;
}

int wsmgr_active(const WorkspaceManager *wm) {
    if (wm == NULL) return -1;
    if (wsmgr_find(wm, wm->active_id) < 0) return -1;
    return wm->active_id;
}

int wsmgr_remove(WorkspaceManager *wm, int ws_id) {
    int idx, i;
    if (wm == NULL) return -1;
    idx = wsmgr_find(wm, ws_id);
    if (idx < 0) return -1;
    wm->workspaces[idx].id = -1;
    wm->workspaces[idx].name[0] = '\0';
    wm->workspaces[idx].session_count = 0;
    wm->count--;
    if (wm->active_id == ws_id) {
        wm->active_id = -1;
        for (i = 0; i < WS_MAX_WORKSPACES; i++) {
            if (wm->workspaces[i].id >= 0) {
                wm->active_id = wm->workspaces[i].id;
                break;
            }
        }
    }
    return 0;
}
