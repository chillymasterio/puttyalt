#ifndef PUTTYALT_SCROLLSTATE_H
#define PUTTYALT_SCROLLSTATE_H

typedef struct {
    int total_lines;
    int visible_lines;
    int scroll_pos;
    int scroll_max;
    int auto_scroll;
    int smooth;
    float velocity;
    float target_pos;
    int thumb_dragging;
    int thumb_hover;
} ScrollState;

void scroll_init(ScrollState *ss);
void scroll_set_content(ScrollState *ss, int total, int visible);
void scroll_to(ScrollState *ss, int pos);
void scroll_by(ScrollState *ss, int delta);
void scroll_to_bottom(ScrollState *ss);
void scroll_to_top(ScrollState *ss);
int  scroll_at_bottom(const ScrollState *ss);
void scroll_update(ScrollState *ss, float dt);
int  scroll_get_thumb_pos(const ScrollState *ss, int track_height);
int  scroll_get_thumb_size(const ScrollState *ss, int track_height);

#endif
