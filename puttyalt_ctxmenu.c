#include "puttyalt_ctxmenu.h"
#include <string.h>
#include <stdio.h>

void ctx_init(CtxMenu *menu, const char *title)
{
    memset(menu, 0, sizeof(*menu));
    if (title) snprintf(menu->title, sizeof(menu->title), "%s", title);
}

int ctx_add_item(CtxMenu *menu, int id, const char *label, const char *shortcut)
{
    if (menu->count >= CTX_MAX_ITEMS) return -1;
    CtxMenuItem *item = &menu->items[menu->count++];
    item->type = CTX_ITEM;
    item->id = id;
    item->enabled = 1;
    snprintf(item->label, sizeof(item->label), "%s", label);
    if (shortcut) snprintf(item->shortcut, sizeof(item->shortcut), "%s", shortcut);
    return 0;
}

int ctx_add_separator(CtxMenu *menu)
{
    if (menu->count >= CTX_MAX_ITEMS) return -1;
    menu->items[menu->count].type = CTX_SEPARATOR;
    menu->count++;
    return 0;
}

void ctx_set_enabled(CtxMenu *menu, int id, int enabled)
{
    for (int i = 0; i < menu->count; i++)
        if (menu->items[i].id == id) { menu->items[i].enabled = enabled; return; }
}

void ctx_set_checked(CtxMenu *menu, int id, int checked)
{
    for (int i = 0; i < menu->count; i++)
        if (menu->items[i].id == id) { menu->items[i].checked = checked; return; }
}

#ifdef _WIN32
#include <windows.h>
int ctx_show(CtxMenu *menu, void *hwnd, int x, int y)
{
    HMENU hm = CreatePopupMenu();
    for (int i = 0; i < menu->count; i++) {
        CtxMenuItem *item = &menu->items[i];
        if (item->type == CTX_SEPARATOR) { AppendMenuA(hm, MF_SEPARATOR, 0, NULL); continue; }
        UINT flags = MF_STRING;
        if (!item->enabled) flags |= MF_GRAYED;
        if (item->checked) flags |= MF_CHECKED;
        char text[128];
        if (item->shortcut[0]) snprintf(text, sizeof(text), "%s\t%s", item->label, item->shortcut);
        else snprintf(text, sizeof(text), "%s", item->label);
        AppendMenuA(hm, flags, item->id, text);
    }
    int cmd = TrackPopupMenu(hm, TPM_RETURNCMD | TPM_RIGHTBUTTON, x, y, 0, (HWND)hwnd, NULL);
    DestroyMenu(hm);
    return cmd;
}
#else
int ctx_show(CtxMenu *menu, void *hwnd, int x, int y) { (void)menu; (void)hwnd; (void)x; (void)y; return 0; }
#endif

static CtxMenu s_term_menu, s_side_menu, s_tab_menu;

CtxMenu *ctx_build_terminal(int connected, int has_selection)
{
    ctx_init(&s_term_menu, "Terminal");
    ctx_add_item(&s_term_menu, 2001, "Copy", "Ctrl+Shift+C");
    ctx_set_enabled(&s_term_menu, 2001, has_selection);
    ctx_add_item(&s_term_menu, 2002, "Paste", "Ctrl+Shift+V");
    ctx_add_item(&s_term_menu, 2003, "Select All", "Ctrl+Shift+A");
    ctx_add_separator(&s_term_menu);
    ctx_add_item(&s_term_menu, 2004, "Find...", "Ctrl+F");
    ctx_add_item(&s_term_menu, 2005, "Clear Scrollback", NULL);
    ctx_add_separator(&s_term_menu);
    ctx_add_item(&s_term_menu, 3002, "Disconnect", NULL);
    ctx_set_enabled(&s_term_menu, 3002, connected);
    ctx_add_item(&s_term_menu, 3001, "Reconnect", NULL);
    ctx_set_enabled(&s_term_menu, 3001, !connected);
    return &s_term_menu;
}

CtxMenu *ctx_build_sidebar(int has_session)
{
    ctx_init(&s_side_menu, "Sessions");
    ctx_add_item(&s_side_menu, 1001, "New Session", "Ctrl+N");
    ctx_add_separator(&s_side_menu);
    ctx_add_item(&s_side_menu, 7001, "Edit Session", NULL);
    ctx_set_enabled(&s_side_menu, 7001, has_session);
    ctx_add_item(&s_side_menu, 7002, "Delete Session", NULL);
    ctx_set_enabled(&s_side_menu, 7002, has_session);
    ctx_add_item(&s_side_menu, 7003, "Duplicate Session", NULL);
    ctx_set_enabled(&s_side_menu, 7003, has_session);
    return &s_side_menu;
}

CtxMenu *ctx_build_tab(int pinned)
{
    ctx_init(&s_tab_menu, "Tab");
    ctx_add_item(&s_tab_menu, 8001, "New Tab", "Ctrl+N");
    ctx_add_item(&s_tab_menu, 8002, pinned ? "Unpin Tab" : "Pin Tab", NULL);
    ctx_add_item(&s_tab_menu, 8003, "Duplicate Tab", "Ctrl+D");
    ctx_add_separator(&s_tab_menu);
    ctx_add_item(&s_tab_menu, 8004, "Close Tab", NULL);
    ctx_add_item(&s_tab_menu, 8005, "Close Other Tabs", NULL);
    return &s_tab_menu;
}
