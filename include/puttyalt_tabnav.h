#ifndef PUTTYALT_TABNAV_H
#define PUTTYALT_TABNAV_H

#define TAB_NAV_MAX_HISTORY 32

typedef struct {
    int history[TAB_NAV_MAX_HISTORY];
    int history_count;
    int history_pos;
    int mru_mode;
    int active;
    int total;
    int switcher_visible;
} TabNav;

void tabnav_init(TabNav *tn);
void tabnav_set_active(TabNav *tn, int idx, int total);
int  tabnav_next(TabNav *tn);
int  tabnav_prev(TabNav *tn);
int  tabnav_mru_next(TabNav *tn);
int  tabnav_goto(TabNav *tn, int idx);
int  tabnav_close_current(TabNav *tn);

#endif
