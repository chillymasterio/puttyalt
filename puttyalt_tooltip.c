#include "puttyalt_tooltip.h"
#include <string.h>

void tooltip_init(Tooltip *tt)
{
    memset(tt, 0, sizeof(*tt));
}

void tooltip_set(Tooltip *tt, const char *text, int x, int y)
{
    if (!text || !text[0]) { tooltip_hide(tt); return; }
    if (strcmp(tt->text, text) == 0 && tt->active) return;

    snprintf(tt->text, sizeof(tt->text), "%s", text);
    tt->x = x;
    tt->y = y;
    tt->active = 1;
    tt->visible = 0;
    tt->hover_start = 0;  /* set on next update */
    tt->opacity = 0.0f;
    /* Estimate size: 7px per char width, 20px height */
    tt->width = (int)strlen(text) * 7 + 16;
    tt->height = 24;
}

void tooltip_hide(Tooltip *tt)
{
    tt->active = 0;
    tt->visible = 0;
    tt->opacity = 0.0f;
}

void tooltip_update(Tooltip *tt, long now_ms)
{
    if (!tt->active) return;

    if (tt->hover_start == 0) tt->hover_start = now_ms;

    long elapsed = now_ms - tt->hover_start;
    if (elapsed >= TOOLTIP_DELAY_MS) {
        tt->visible = 1;
        long fade_elapsed = elapsed - TOOLTIP_DELAY_MS;
        if (fade_elapsed < TOOLTIP_FADE_MS)
            tt->opacity = (float)fade_elapsed / (float)TOOLTIP_FADE_MS;
        else
            tt->opacity = 1.0f;
    }
}

int tooltip_should_show(const Tooltip *tt)
{
    return tt->visible && tt->opacity > 0.01f;
}
