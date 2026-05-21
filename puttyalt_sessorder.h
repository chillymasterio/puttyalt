#ifndef PUTTYALT_SESSORDER_H
#define PUTTYALT_SESSORDER_H

#define SESS_ORDER_MAX 256

typedef struct {
    char session_ids[SESS_ORDER_MAX][64];
    int positions[SESS_ORDER_MAX];
    int count;
    int dirty;
} SessionOrder;

int  sessorder_init(SessionOrder *so);
int  sessorder_add(SessionOrder *so, const char *id, int position);
int  sessorder_move(SessionOrder *so, const char *id, int new_pos);
int  sessorder_remove(SessionOrder *so, const char *id);
int  sessorder_get_pos(const SessionOrder *so, const char *id);
int  sessorder_save(const SessionOrder *so, const char *path);
int  sessorder_load(SessionOrder *so, const char *path);
void sessorder_destroy(SessionOrder *so);

#endif
