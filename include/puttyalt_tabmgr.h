#ifndef PUTTYALT_TABMGR_H
#define PUTTYALT_TABMGR_H

#define TAB_MAX 32

typedef struct {
    int id;
    char title[128];
    char host[256];
    int port;
    int connected;
    int pinned;
    int color_index;
    int dirty;
    unsigned long create_time;
} TabInfo;

typedef struct {
    TabInfo tabs[TAB_MAX];
    int tab_count;
    int active_tab;
    int max_tabs;
    int show_close_btn;
    int double_click_closes;
    int new_tab_position;
} TabManager;

void tabmgr_init(TabManager *tm);
int  tabmgr_add(TabManager *tm, const char *title);
int  tabmgr_close(TabManager *tm, int index);
int  tabmgr_close_others(TabManager *tm, int except_index);
int  tabmgr_close_all(TabManager *tm);
void tabmgr_activate(TabManager *tm, int index);
void tabmgr_move(TabManager *tm, int from, int to);
void tabmgr_pin(TabManager *tm, int index, int pinned);
void tabmgr_set_title(TabManager *tm, int index, const char *title);
void tabmgr_next(TabManager *tm);
void tabmgr_prev(TabManager *tm);
TabInfo *tabmgr_get(TabManager *tm, int index);
TabInfo *tabmgr_active(TabManager *tm);

#endif
