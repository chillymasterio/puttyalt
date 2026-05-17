#include "puttyalt_sessgroup_v2.h"
#include <string.h>

void grpstore_init(GroupStore *gs) { memset(gs, 0, sizeof(*gs)); gs->next_id = 1; }

int grpstore_add(GroupStore *gs, const char *name, int parent_id, int color)
{
    if (gs->count >= GROUP_MAX) return -1;
    if (parent_id > 0 && grpstore_depth(gs, parent_id) >= GROUP_NEST_MAX) return -1;
    SessionGroup *g = &gs->groups[gs->count++];
    memset(g, 0, sizeof(*g));
    g->id = gs->next_id++;
    snprintf(g->name, sizeof(g->name), "%s", name);
    g->parent_id = parent_id;
    g->color = color;
    return g->id;
}

int grpstore_remove(GroupStore *gs, int id)
{
    for (int i = 0; i < gs->count; i++) {
        if (gs->groups[i].id == id) {
            /* reparent children to removed group's parent */
            int pid = gs->groups[i].parent_id;
            for (int j = 0; j < gs->count; j++)
                if (gs->groups[j].parent_id == id) gs->groups[j].parent_id = pid;
            memmove(&gs->groups[i], &gs->groups[i+1], (gs->count-i-1)*sizeof(SessionGroup));
            gs->count--;
            return 0;
        }
    }
    return -1;
}

int grpstore_move(GroupStore *gs, int id, int new_parent)
{
    SessionGroup *g = grpstore_find(gs, id);
    if (!g) return -1;
    /* prevent cycles */
    int p = new_parent;
    while (p > 0) {
        if (p == id) return -1;
        SessionGroup *pg = grpstore_find(gs, p);
        p = pg ? pg->parent_id : 0;
    }
    g->parent_id = new_parent;
    return 0;
}

int grpstore_add_session(GroupStore *gs, int group_id, int session_id)
{
    SessionGroup *g = grpstore_find(gs, group_id);
    if (!g || g->session_count >= 64) return -1;
    g->session_ids[g->session_count++] = session_id;
    return 0;
}

int grpstore_remove_session(GroupStore *gs, int group_id, int session_id)
{
    SessionGroup *g = grpstore_find(gs, group_id);
    if (!g) return -1;
    for (int i = 0; i < g->session_count; i++) {
        if (g->session_ids[i] == session_id) {
            memmove(&g->session_ids[i], &g->session_ids[i+1], (g->session_count-i-1)*sizeof(int));
            g->session_count--;
            return 0;
        }
    }
    return -1;
}

SessionGroup *grpstore_find(GroupStore *gs, int id)
{
    for (int i = 0; i < gs->count; i++) if (gs->groups[i].id == id) return &gs->groups[i];
    return NULL;
}

int grpstore_children(GroupStore *gs, int parent_id, int *ids, int max)
{
    int n = 0;
    for (int i = 0; i < gs->count && n < max; i++)
        if (gs->groups[i].parent_id == parent_id) ids[n++] = gs->groups[i].id;
    return n;
}

void grpstore_toggle_collapse(GroupStore *gs, int id)
{
    SessionGroup *g = grpstore_find(gs, id);
    if (g) g->collapsed = !g->collapsed;
}

int grpstore_depth(GroupStore *gs, int id)
{
    int d = 0;
    SessionGroup *g = grpstore_find(gs, id);
    while (g && g->parent_id > 0 && d < GROUP_NEST_MAX + 1) {
        d++;
        g = grpstore_find(gs, g->parent_id);
    }
    return d;
}
