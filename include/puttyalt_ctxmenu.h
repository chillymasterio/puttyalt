#ifndef PUTTYALT_CTXMENU_H
#define PUTTYALT_CTXMENU_H

#define CTX_MAX_ITEMS 32

typedef enum { CTX_ITEM, CTX_SEPARATOR, CTX_SUBMENU } CtxItemType;

typedef struct {
    CtxItemType type;
    int id;
    char label[64];
    char shortcut[32];
    int enabled;
    int checked;
    int icon_index;
} CtxMenuItem;

typedef struct {
    CtxMenuItem items[CTX_MAX_ITEMS];
    int count;
    char title[64];
} CtxMenu;

void ctx_init(CtxMenu *menu, const char *title);
int  ctx_add_item(CtxMenu *menu, int id, const char *label, const char *shortcut);
int  ctx_add_separator(CtxMenu *menu);
void ctx_set_enabled(CtxMenu *menu, int id, int enabled);
void ctx_set_checked(CtxMenu *menu, int id, int checked);
int  ctx_show(CtxMenu *menu, void *hwnd, int x, int y);

CtxMenu *ctx_build_terminal(int connected, int has_selection);
CtxMenu *ctx_build_sidebar(int has_session);
CtxMenu *ctx_build_tab(int pinned);

#endif
