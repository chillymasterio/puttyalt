#include "puttyalt_sessgroup.h"
#include <string.h>
#include <stdio.h>

void sessgroup_init(SessionGroupMgr *gm) { memset(gm, 0, sizeof(*gm)); }

int sessgroup_create(SessionGroupMgr *gm, const char *name, const char *color)
{
    if (gm->count >= GROUP_MAX_GROUPS) return -1;
    if (!name || name[0] == '\0') return -1;
    SessionGroup *g = &gm->groups[gm->count];
    memset(g, 0, sizeof(*g));
    snprintf(g->name, sizeof(g->name), "%s", name);
    if (color) snprintf(g->color, sizeof(g->color), "%s", color);
    g->sort_order = gm->count;
    gm->count++;
    return 0;
}

int sessgroup_delete(SessionGroupMgr *gm, const char *name)
{
    for (int i = 0; i < gm->count; i++) {
        if (strcmp(gm->groups[i].name, name) == 0) {
            for (int j = i; j < gm->count - 1; j++)
                gm->groups[j] = gm->groups[j + 1];
            gm->count--;
            return 0;
        }
    }
    return -1;
}

int sessgroup_rename(SessionGroupMgr *gm, const char *old_name, const char *new_name)
{
    SessionGroup *g = sessgroup_find(gm, old_name);
    if (!g || !new_name) return -1;
    snprintf(g->name, sizeof(g->name), "%s", new_name);
    return 0;
}

int sessgroup_add_session(SessionGroupMgr *gm, const char *group, int session_id)
{
    SessionGroup *g = sessgroup_find(gm, group);
    if (!g || g->count >= GROUP_MAX_SESSIONS) return -1;
    for (int i = 0; i < g->count; i++)
        if (g->session_ids[i] == session_id) return 0; /* already there */
    g->session_ids[g->count++] = session_id;
    return 0;
}

int sessgroup_remove_session(SessionGroupMgr *gm, const char *group, int session_id)
{
    SessionGroup *g = sessgroup_find(gm, group);
    if (!g) return -1;
    for (int i = 0; i < g->count; i++) {
        if (g->session_ids[i] == session_id) {
            for (int j = i; j < g->count - 1; j++)
                g->session_ids[j] = g->session_ids[j + 1];
            g->count--;
            return 0;
        }
    }
    return -1;
}

SessionGroup *sessgroup_find(SessionGroupMgr *gm, const char *name)
{
    for (int i = 0; i < gm->count; i++)
        if (strcmp(gm->groups[i].name, name) == 0)
            return &gm->groups[i];
    return NULL;
}

int sessgroup_toggle_collapse(SessionGroupMgr *gm, const char *name)
{
    SessionGroup *g = sessgroup_find(gm, name);
    if (!g) return -1;
    g->collapsed = !g->collapsed;
    return 0;
}
