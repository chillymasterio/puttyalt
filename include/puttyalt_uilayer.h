#ifndef PUTTYALT_UILAYER_H
#define PUTTYALT_UILAYER_H
#include <stdint.h>

/* UI abstraction layer for future framework migration.
 * Provides a clean interface between app logic and rendering backend.
 * Current backend: Win32 GDI. Planned: Direct2D / SDL2 / custom GPU. */

typedef struct { int x, y, w, h; } UIRect;
typedef struct { uint8_t r, g, b, a; } UIColor;

typedef enum {
    UI_WIDGET_LABEL,
    UI_WIDGET_BUTTON,
    UI_WIDGET_INPUT,
    UI_WIDGET_CHECKBOX,
    UI_WIDGET_LISTBOX,
    UI_WIDGET_PANEL,
    UI_WIDGET_TABS,
    UI_WIDGET_PROGRESS,
    UI_WIDGET_SEPARATOR,
    UI_WIDGET_CUSTOM,
} UIWidgetType;

typedef struct UIWidget {
    UIWidgetType type;
    int id;
    UIRect rect;
    char text[256];
    int visible;
    int enabled;
    int focused;
    int hovered;
    UIColor fg, bg;
    void *native_handle;
    void (*on_click)(struct UIWidget *w, void *ctx);
    void (*on_change)(struct UIWidget *w, void *ctx);
    void *user_data;
    struct UIWidget *parent;
    struct UIWidget *children;
    struct UIWidget *next;
} UIWidget;

typedef struct {
    UIWidget *root;
    UIWidget *focused;
    UIWidget *hovered;
    int widget_count;
    UIColor theme_bg;
    UIColor theme_fg;
    UIColor theme_accent;
    UIColor theme_border;
    int scale_pct; /* 100 = normal, 125 = 125% DPI */
    int dirty;
} UIContext;

void ui_ctx_init(UIContext *ctx);
void ui_ctx_destroy(UIContext *ctx);
UIWidget *ui_create(UIContext *ctx, UIWidgetType type, int id, const char *text);
void ui_add_child(UIWidget *parent, UIWidget *child);
void ui_set_rect(UIWidget *w, int x, int y, int width, int height);
void ui_set_visible(UIWidget *w, int visible);
void ui_set_enabled(UIWidget *w, int enabled);
void ui_set_text(UIWidget *w, const char *text);
void ui_invalidate(UIContext *ctx);

#endif
