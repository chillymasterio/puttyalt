#include "puttyalt_tooltip.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void tooltip_init(Tooltip *tt) { memset(tt, 0, sizeof(*tt)); tt->delay_ms = 500; tt->duration_ms = 5000; }

void tooltip_show(Tooltip *tt, const char *text, int x, int y)
{
    snprintf(tt->text, sizeof(tt->text), "%s", text);
    tt->x = x; tt->y = y;
    tt->show_time = (unsigned long)(clock() * 1000 / CLOCKS_PER_SEC);
    tt->visible = 0; /* will become visible after delay */
}

void tooltip_hide(Tooltip *tt) { tt->visible = 0; tt->text[0] = '\0'; }

void tooltip_update(Tooltip *tt)
{
    if (!tt->text[0]) return;
    unsigned long now = (unsigned long)(clock() * 1000 / CLOCKS_PER_SEC);
    unsigned long elapsed = now - tt->show_time;
    if (!tt->visible && elapsed >= (unsigned long)tt->delay_ms) tt->visible = 1;
    if (tt->visible && elapsed >= (unsigned long)(tt->delay_ms + tt->duration_ms)) tooltip_hide(tt);
}

int tooltip_should_show(Tooltip *tt) { return tt->visible && tt->text[0]; }
