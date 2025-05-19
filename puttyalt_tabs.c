/*
 * puttyalt_tabs.c: Tab management implementation.
 */

#include "puttyalt_tabs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void tabs_init(TabManager *tm)
{
    memset(tm, 0, sizeof(*tm));
    tm->active = -1;
    tm->show_tab_bar = 1;
    tm->tab_bar_position = 0;
    tm->close_on_exit = 1;
    tm->confirm_close = 1;
}

int tabs_add(TabManager *tm, const char *title, const char *hostname, int port)
{
    if (tm->count >= MAX_TABS)
        return -1;

    TabEntry *t = &tm->tabs[tm->count];
    memset(t, 0, sizeof(*t));
    t->id = tm->count;
    t->port = port;
    t->state = TAB_STATE_EMPTY;
    t->created_at = (long)time(NULL);
    t->last_activity = t->created_at;

    if (title)
        snprintf(t->title, sizeof(t->title), "%s", title);
    else if (hostname)
        snprintf(t->title, sizeof(t->title), "%s:%d", hostname, port);

    if (hostname)
        snprintf(t->hostname, sizeof(t->hostname), "%s", hostname);

    if (tm->active < 0)
        tm->active = tm->count;

    return tm->count++;
}

int tabs_close(TabManager *tm, int index)
{
    if (index < 0 || index >= tm->count)
        return -1;

    /* Don't close pinned tabs */
    if (tm->tabs[index].pinned)
        return -1;

    /* Shift tabs down */
    for (int i = index; i < tm->count - 1; i++)
        tm->tabs[i] = tm->tabs[i + 1];
    memset(&tm->tabs[tm->count - 1], 0, sizeof(TabEntry));
    tm->count--;

    /* Adjust active index — prevent underflow on empty manager */
    if (tm->count == 0) {
        tm->active = 0;
    } else if (tm->active >= tm->count) {
        tm->active = tm->count - 1;
    }

    return 0;
}

int tabs_close_all_disconnected(TabManager *tm)
{
    int closed = 0;
    for (int i = tm->count - 1; i >= 0; i--) {
        if (tm->tabs[i].state == TAB_STATE_DISCONNECTED ||
            tm->tabs[i].state == TAB_STATE_ERROR) {
            if (!tm->tabs[i].pinned) {
                tabs_close(tm, i);
                closed++;
            }
        }
    }
    return closed;
}

void tabs_switch(TabManager *tm, int index)
{
    if (index >= 0 && index < tm->count) {
        tm->active = index;
        tm->tabs[index].unread = 0;
    }
}

void tabs_next(TabManager *tm)
{
    if (tm->count <= 1)
        return;
    int next = (tm->active + 1) % tm->count;
    tabs_switch(tm, next);
}

void tabs_prev(TabManager *tm)
{
    if (tm->count <= 1)
        return;
    int prev = (tm->active - 1 + tm->count) % tm->count;
    tabs_switch(tm, prev);
}

int tabs_move(TabManager *tm, int from, int to)
{
    if (from < 0 || from >= tm->count || to < 0 || to >= tm->count)
        return -1;
    if (from == to)
        return 0;

    TabEntry tmp = tm->tabs[from];
    if (from < to) {
        for (int i = from; i < to; i++)
            tm->tabs[i] = tm->tabs[i + 1];
    } else {
        for (int i = from; i > to; i--)
            tm->tabs[i] = tm->tabs[i - 1];
    }
    tm->tabs[to] = tmp;

    if (tm->active == from)
        tm->active = to;

    return 0;
}

void tabs_set_title(TabManager *tm, int index, const char *title)
{
    if (index >= 0 && index < tm->count && title)
        snprintf(tm->tabs[index].title, sizeof(tm->tabs[index].title), "%s", title);
}

void tabs_mark_activity(TabManager *tm, int index)
{
    if (index >= 0 && index < tm->count) {
        tm->tabs[index].last_activity = (long)time(NULL);
        if (index != tm->active)
            tm->tabs[index].unread = 1;
    }
}

int tabs_find_by_hostname(const TabManager *tm, const char *hostname)
{
    for (int i = 0; i < tm->count; i++) {
        if (strcmp(tm->tabs[i].hostname, hostname) == 0)
            return i;
    }
    return -1;
}

void tabs_pin(TabManager *tm, int index, int pinned)
{
    if (index >= 0 && index < tm->count)
        tm->tabs[index].pinned = pinned ? 1 : 0;
}
