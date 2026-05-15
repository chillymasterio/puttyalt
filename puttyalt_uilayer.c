#include "puttyalt_uilayer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ui_ctx_init(UIContext *ctx)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->theme_bg = (UIColor){27, 40, 56, 255};
    ctx->theme_fg = (UIColor){232, 237, 242, 255};
    ctx->theme_accent = (UIColor){74, 158, 224, 255};
    ctx->theme_border = (UIColor){58, 79, 106, 255};
    ctx->scale_pct = 100;
}

void ui_ctx_destroy(UIContext *ctx)
{
    /* Walk tree and free all widgets */
    (void)ctx;
}

UIWidget *ui_create(UIContext *ctx, UIWidgetType type, int id, const char *text)
{
    UIWidget *w = calloc(1, sizeof(UIWidget));
    if (!w) return NULL;
    w->type = type;
    w->id = id;
    w->visible = 1;
    w->enabled = 1;
    w->fg = ctx->theme_fg;
    w->bg = ctx->theme_bg;
    if (text) snprintf(w->text, sizeof(w->text), "%s", text);
    ctx->widget_count++;
    return w;
}

void ui_add_child(UIWidget *parent, UIWidget *child)
{
    if (!parent || !child) return;
    child->parent = parent;
    child->next = parent->children;
    parent->children = child;
}

void ui_set_rect(UIWidget *w, int x, int y, int width, int height)
{
    if (w) { w->rect = (UIRect){x, y, width, height}; }
}

void ui_set_visible(UIWidget *w, int visible) { if (w) w->visible = visible; }
void ui_set_enabled(UIWidget *w, int enabled) { if (w) w->enabled = enabled; }

void ui_set_text(UIWidget *w, const char *text)
{
    if (w && text) snprintf(w->text, sizeof(w->text), "%s", text);
}

void ui_invalidate(UIContext *ctx) { ctx->dirty = 1; }
