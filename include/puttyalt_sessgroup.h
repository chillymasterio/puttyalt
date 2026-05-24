#ifndef PUTTYALT_SESSGROUP_H
#define PUTTYALT_SESSGROUP_H

#define GROUP_MAX_GROUPS   32
#define GROUP_MAX_SESSIONS 64
#define GROUP_NAME_LEN     64

typedef struct {
    char name[GROUP_NAME_LEN];
    char color[8];  /* hex color */
    int session_ids[GROUP_MAX_SESSIONS];
    int count;
    int collapsed;
    int sort_order;
} SessionGroup;

typedef struct {
    SessionGroup groups[GROUP_MAX_GROUPS];
    int count;
    int active_group;
} SessionGroupMgr;

void sessgroup_init(SessionGroupMgr *gm);
int  sessgroup_create(SessionGroupMgr *gm, const char *name, const char *color);
int  sessgroup_delete(SessionGroupMgr *gm, const char *name);
int  sessgroup_rename(SessionGroupMgr *gm, const char *old_name, const char *new_name);
int  sessgroup_add_session(SessionGroupMgr *gm, const char *group, int session_id);
int  sessgroup_remove_session(SessionGroupMgr *gm, const char *group, int session_id);
SessionGroup *sessgroup_find(SessionGroupMgr *gm, const char *name);
int  sessgroup_toggle_collapse(SessionGroupMgr *gm, const char *name);

#endif
