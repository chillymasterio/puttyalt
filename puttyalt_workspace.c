#include <string.h>
#include <stdio.h>
#include "puttyalt_workspace.h"

void ws_init(WorkspaceManager *wm)
{
    memset(wm, 0, sizeof(*wm));
    wm->current = -1;
}

int ws_create(WorkspaceManager *wm, const char *name)
{
    if (wm->num_workspaces >= WS_MAX_WORKSPACES)
        return -1;
    Workspace *w = &wm->workspaces[wm->num_workspaces];
    memset(w, 0, sizeof(*w));
    strncpy(w->name, name, WS_NAME_LEN - 1);
    if (wm->current < 0) wm->current = wm->num_workspaces;
    return wm->num_workspaces++;
}

int ws_delete(WorkspaceManager *wm, int index)
{
    if (index < 0 || index >= wm->num_workspaces) return -1;
    for (int i = index; i < wm->num_workspaces - 1; i++)
        wm->workspaces[i] = wm->workspaces[i + 1];
    wm->num_workspaces--;
    if (wm->current >= wm->num_workspaces)
        wm->current = wm->num_workspaces - 1;
    return 0;
}

int ws_switch(WorkspaceManager *wm, int index)
{
    if (index < 0 || index >= wm->num_workspaces) return -1;
    wm->workspaces[wm->current].active = 0;
    wm->current = index;
    wm->workspaces[index].active = 1;
    return 0;
}

int ws_add_session(WorkspaceManager *wm, int ws_idx, const char *host,
                   int port, const char *user)
{
    if (ws_idx < 0 || ws_idx >= wm->num_workspaces) return -1;
    Workspace *w = &wm->workspaces[ws_idx];
    if (w->num_sessions >= WS_MAX_SESSIONS) return -1;
    WorkspaceSession *s = &w->sessions[w->num_sessions];
    strncpy(s->host, host, sizeof(s->host) - 1);
    s->port = port;
    if (user) strncpy(s->username, user, sizeof(s->username) - 1);
    s->tab_index = -1;
    w->num_sessions++;
    return 0;
}

int ws_remove_session(WorkspaceManager *wm, int ws_idx, int sess_idx)
{
    if (ws_idx < 0 || ws_idx >= wm->num_workspaces) return -1;
    Workspace *w = &wm->workspaces[ws_idx];
    if (sess_idx < 0 || sess_idx >= w->num_sessions) return -1;
    for (int i = sess_idx; i < w->num_sessions - 1; i++)
        w->sessions[i] = w->sessions[i + 1];
    w->num_sessions--;
    return 0;
}

int ws_save(const WorkspaceManager *wm, const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) return -1;
    fprintf(fp, "workspaces=%d\n", wm->num_workspaces);
    for (int i = 0; i < wm->num_workspaces; i++) {
        const Workspace *w = &wm->workspaces[i];
        fprintf(fp, "[workspace:%d]\nname=%s\nsessions=%d\n",
                i, w->name, w->num_sessions);
        for (int j = 0; j < w->num_sessions; j++)
            fprintf(fp, "host=%s\nport=%d\nuser=%s\n",
                    w->sessions[j].host, w->sessions[j].port,
                    w->sessions[j].username);
    }
    fclose(fp);
    return 0;
}

int ws_load(WorkspaceManager *wm, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    /* Simplified: real implementation would parse INI format */
    fclose(fp);
    return 0;
}

int ws_rename(WorkspaceManager *wm, int index, const char *new_name)
{
    if (index < 0 || index >= wm->num_workspaces) return -1;
    strncpy(wm->workspaces[index].name, new_name, WS_NAME_LEN - 1);
    wm->workspaces[index].name[WS_NAME_LEN - 1] = '\0';
    return 0;
}
