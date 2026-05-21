#include "puttyalt_scrollstate.h"

void scroll_init(ScrollState *ss)
{
    ss->total_lines = 0;
    ss->visible_lines = 24;
    ss->scroll_pos = 0;
    ss->scroll_max = 0;
    ss->auto_scroll = 1;
    ss->smooth = 1;
    ss->velocity = 0.0f;
    ss->target_pos = 0.0f;
    ss->thumb_dragging = 0;
    ss->thumb_hover = 0;
}

void scroll_set_content(ScrollState *ss, int total, int visible)
{
    int was_at_bottom = scroll_at_bottom(ss);
    ss->total_lines = total;
    ss->visible_lines = visible > 0 ? visible : 1;
    ss->scroll_max = total > visible ? total - visible : 0;

    if (ss->scroll_pos > ss->scroll_max)
        ss->scroll_pos = ss->scroll_max;

    /* Preserve scroll at bottom when content grows */
    if (was_at_bottom && ss->auto_scroll)
        ss->scroll_pos = ss->scroll_max;

    ss->target_pos = (float)ss->scroll_pos;
}

void scroll_to(ScrollState *ss, int pos)
{
    if (pos < 0) pos = 0;
    if (pos > ss->scroll_max) pos = ss->scroll_max;
    ss->scroll_pos = pos;
    ss->target_pos = (float)pos;
}

void scroll_by(ScrollState *ss, int delta)
{
    scroll_to(ss, ss->scroll_pos + delta);
}

void scroll_to_bottom(ScrollState *ss)
{
    scroll_to(ss, ss->scroll_max);
}

void scroll_to_top(ScrollState *ss)
{
    scroll_to(ss, 0);
}

int scroll_at_bottom(const ScrollState *ss)
{
    return ss->scroll_pos >= ss->scroll_max;
}

void scroll_update(ScrollState *ss, float dt)
{
    if (!ss->smooth) return;
    float diff = ss->target_pos - (float)ss->scroll_pos;
    if (diff > 0.5f || diff < -0.5f) {
        ss->velocity = diff * 8.0f * dt;
        float new_pos = (float)ss->scroll_pos + ss->velocity;
        ss->scroll_pos = (int)(new_pos + 0.5f);
        if (ss->scroll_pos < 0) ss->scroll_pos = 0;
        if (ss->scroll_pos > ss->scroll_max) ss->scroll_pos = ss->scroll_max;
    } else {
        ss->scroll_pos = (int)ss->target_pos;
        ss->velocity = 0.0f;
    }
}

int scroll_get_thumb_pos(const ScrollState *ss, int track_height)
{
    if (ss->scroll_max <= 0) return 0;
    return (ss->scroll_pos * track_height) / (ss->scroll_max + ss->visible_lines);
}

int scroll_get_thumb_size(const ScrollState *ss, int track_height)
{
    if (ss->total_lines <= 0) return track_height;
    int size = (ss->visible_lines * track_height) / ss->total_lines;
    if (size < 20) size = 20;
    if (size > track_height) size = track_height;
    return size;
}
