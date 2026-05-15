#include "puttyalt_tabmgr.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void tabmgr_init(TabManager *tm)
{
    memset(tm, 0, sizeof(*tm));
    tm->max_tabs = TAB_MAX;
    tm->show_close_btn = 1;
    tm->new_tab_position = 0;
}

int tabmgr_add(TabManager *tm, const char *title)
{
    if (tm->tab_count >= tm->max_tabs) return -1;
    int idx = tm->tab_count;
    if (tm->new_tab_position == 1 && tm->tab_count > 0) {
        idx = tm->active_tab + 1;
        memmove(&tm->tabs[idx + 1], &tm->tabs[idx], (tm->tab_count - idx) * sizeof(TabInfo));
    }
    TabInfo *t = &tm->tabs[idx];
    memset(t, 0, sizeof(*t));
    t->id = tm->tab_count + 1;
    snprintf(t->title, sizeof(t->title), "%s", title ? title : "New Session");
    t->create_time = (unsigned long)time(NULL);
    tm->tab_count++;
    tm->active_tab = idx;
    return idx;
}

int tabmgr_close(TabManager *tm, int index)
{
    if (index < 0 || index >= tm->tab_count) return -1;
    if (tm->tabs[index].pinned) return -1;
    memmove(&tm->tabs[index], &tm->tabs[index + 1], (tm->tab_count - index - 1) * sizeof(TabInfo));
    tm->tab_count--;
    if (tm->active_tab >= tm->tab_count) tm->active_tab = tm->tab_count - 1;
    if (tm->active_tab < 0) tm->active_tab = 0;
    return 0;
}

int tabmgr_close_others(TabManager *tm, int except)
{
    if (except < 0 || except >= tm->tab_count) return -1;
    TabInfo keep = tm->tabs[except];
    tm->tabs[0] = keep;
    tm->tab_count = 1;
    tm->active_tab = 0;
    return 0;
}

int tabmgr_close_all(TabManager *tm) { tm->tab_count = 0; tm->active_tab = 0; return 0; }
void tabmgr_activate(TabManager *tm, int index) { if (index >= 0 && index < tm->tab_count) tm->active_tab = index; }

void tabmgr_move(TabManager *tm, int from, int to)
{
    if (from < 0 || from >= tm->tab_count || to < 0 || to >= tm->tab_count || from == to) return;
    TabInfo tmp = tm->tabs[from];
    if (from < to) memmove(&tm->tabs[from], &tm->tabs[from + 1], (to - from) * sizeof(TabInfo));
    else memmove(&tm->tabs[to + 1], &tm->tabs[to], (from - to) * sizeof(TabInfo));
    tm->tabs[to] = tmp;
    tm->active_tab = to;
}

void tabmgr_pin(TabManager *tm, int index, int pinned) { if (index >= 0 && index < tm->tab_count) tm->tabs[index].pinned = pinned; }
void tabmgr_set_title(TabManager *tm, int index, const char *title) { if (index >= 0 && index < tm->tab_count) snprintf(tm->tabs[index].title, sizeof(tm->tabs[index].title), "%s", title); }
void tabmgr_next(TabManager *tm) { tm->active_tab = (tm->active_tab + 1) % tm->tab_count; }
void tabmgr_prev(TabManager *tm) { tm->active_tab = (tm->active_tab - 1 + tm->tab_count) % tm->tab_count; }
TabInfo *tabmgr_get(TabManager *tm, int index) { return (index >= 0 && index < tm->tab_count) ? &tm->tabs[index] : NULL; }
TabInfo *tabmgr_active(TabManager *tm) { return tabmgr_get(tm, tm->active_tab); }
