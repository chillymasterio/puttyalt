#ifndef PUTTYALT_WORKSPACE_H
#define PUTTYALT_WORKSPACE_H

#define WS_MAX_WORKSPACES  8
#define WS_MAX_SESSIONS    32
#define WS_NAME_LEN        64

typedef struct WorkspaceSession {
    char host[256];
    int port;
    char username[128];
    int tab_index;
} WorkspaceSession;

typedef struct Workspace {
    char name[WS_NAME_LEN];
    WorkspaceSession sessions[WS_MAX_SESSIONS];
    int num_sessions;
    int active;
} Workspace;

typedef struct WorkspaceManager {
    Workspace workspaces[WS_MAX_WORKSPACES];
    int num_workspaces;
    int current;
} WorkspaceManager;

void ws_init(WorkspaceManager *wm);
int  ws_create(WorkspaceManager *wm, const char *name);
int  ws_delete(WorkspaceManager *wm, int index);
int  ws_switch(WorkspaceManager *wm, int index);
int  ws_add_session(WorkspaceManager *wm, int ws_idx, const char *host,
                    int port, const char *user);
int  ws_remove_session(WorkspaceManager *wm, int ws_idx, int sess_idx);
int  ws_save(const WorkspaceManager *wm, const char *path);
int  ws_load(WorkspaceManager *wm, const char *path);
int  ws_rename(WorkspaceManager *wm, int index, const char *new_name);

#endif
