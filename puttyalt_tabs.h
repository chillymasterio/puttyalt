/*
 * puttyalt_tabs.h: Tab management for multiple sessions in one window.
 */

#ifndef PUTTYALT_TABS_H
#define PUTTYALT_TABS_H

#define MAX_TABS 64

typedef enum {
    TAB_STATE_EMPTY,
    TAB_STATE_CONNECTING,
    TAB_STATE_CONNECTED,
    TAB_STATE_DISCONNECTED,
    TAB_STATE_ERROR
} TabState;

typedef struct TabEntry {
    int          id;
    char         title[256];
    char         hostname[256];
    int          port;
    TabState     state;
    int          unread;        /* unread output flag for inactive tabs */
    int          pinned;        /* pinned tabs can't be closed accidentally */
    long         created_at;
    long         last_activity;
    void        *terminal_ctx;  /* opaque pointer to terminal state */
    void        *backend_ctx;   /* opaque pointer to backend handle */
} TabEntry;

typedef struct TabManager {
    TabEntry tabs[MAX_TABS];
    int      count;
    int      active;           /* index of currently visible tab */
    int      show_tab_bar;     /* 0 = auto-hide when 1 tab */
    int      tab_bar_position; /* 0 = top, 1 = bottom */
    int      close_on_exit;    /* close tab when session ends */
    int      confirm_close;    /* ask before closing connected tab */
} TabManager;

void tabs_init(TabManager *tm);
int  tabs_add(TabManager *tm, const char *title, const char *hostname, int port);
int  tabs_close(TabManager *tm, int index);
int  tabs_close_all_disconnected(TabManager *tm);
void tabs_switch(TabManager *tm, int index);
void tabs_next(TabManager *tm);
void tabs_prev(TabManager *tm);
int  tabs_move(TabManager *tm, int from, int to);
void tabs_set_title(TabManager *tm, int index, const char *title);
void tabs_mark_activity(TabManager *tm, int index);
int  tabs_find_by_hostname(const TabManager *tm, const char *hostname);
void tabs_pin(TabManager *tm, int index, int pinned);

#endif /* PUTTYALT_TABS_H */
