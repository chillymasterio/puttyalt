#include "puttyalt_focusring.h"
#include "puttyalt_design.h"

void focus_ring_init(FocusRing *fr)
{
    fr->current = FOCUS_TERMINAL;
    fr->previous = FOCUS_NONE;
    fr->ring_visible = 1;
    fr->ring_width = 2;
    fr->ring_color = DS_BORDER_FOCUS;
    fr->animate = 1;
    fr->opacity = 1.0f;
}

void focus_ring_set(FocusRing *fr, FocusTarget target)
{
    fr->previous = fr->current;
    fr->current = target;
}

void focus_ring_cycle(FocusRing *fr)
{
    fr->previous = fr->current;
    switch (fr->current) {
    case FOCUS_TERMINAL: fr->current = FOCUS_SIDEBAR; break;
    case FOCUS_SIDEBAR: fr->current = FOCUS_TABBAR; break;
    case FOCUS_TABBAR: fr->current = FOCUS_TERMINAL; break;
    default: fr->current = FOCUS_TERMINAL; break;
    }
}

FocusTarget focus_ring_get(const FocusRing *fr)
{
    return fr->current;
}

int focus_ring_should_draw(const FocusRing *fr, FocusTarget panel)
{
    return fr->ring_visible && fr->current == panel;
}
