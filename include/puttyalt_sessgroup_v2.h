#ifndef PUTTYALT_SESSGROUP_V2_H
#define PUTTYALT_SESSGROUP_V2_H

#define GROUP_MAX 32
#define GROUP_NEST_MAX 4

typedef struct SessionGroup {
    int id;
    char name[64];
    int parent_id;
    int color;
    int collapsed;
    int session_ids[64];
    int session_count;
    int sort_order;
} SessionGroup;

typedef struct {
    SessionGroup groups[GROUP_MAX];
    int count;
    int next_id;
} GroupStore;

void grpstore_init(GroupStore *gs);
int  grpstore_add(GroupStore *gs, const char *name, int parent_id, int color);
int  grpstore_remove(GroupStore *gs, int id);
int  grpstore_move(GroupStore *gs, int id, int new_parent);
int  grpstore_add_session(GroupStore *gs, int group_id, int session_id);
int  grpstore_remove_session(GroupStore *gs, int group_id, int session_id);
SessionGroup *grpstore_find(GroupStore *gs, int id);
int  grpstore_children(GroupStore *gs, int parent_id, int *ids, int max);
void grpstore_toggle_collapse(GroupStore *gs, int id);
int  grpstore_depth(GroupStore *gs, int id);

#endif
