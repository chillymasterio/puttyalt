/* puttyalt_splitbar.c - Splitter bar hit-testing + drag handling. */
#include <string.h>
typedef struct {
    int position;
    int orientation;
    int min_pos;
    int max_pos;
    int thickness;
    int dragging;
    int drag_offset;
} SplitBar;
void splitbar_init(SplitBar *s, int orientation, int min_pos, int max_pos) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
    s->orientation = orientation;
    s->min_pos = min_pos;
    s->max_pos = max_pos;
    s->position = (min_pos + max_pos) / 2;
    s->thickness = 4;
}
int splitbar_hit(const SplitBar *s, int x, int y) {
    if (!s) return 0;
    int coord = s->orientation == 0 ? x : y;
    return (coord >= s->position - s->thickness / 2 - 1 &&
            coord <= s->position + s->thickness / 2 + 1) ? 1 : 0;
}
int splitbar_begin_drag(SplitBar *s, int x, int y) {
    if (!s || !splitbar_hit(s, x, y)) return -1;
    int coord = s->orientation == 0 ? x : y;
    s->dragging = 1;
    s->drag_offset = coord - s->position;
    return 0;
}
int splitbar_drag(SplitBar *s, int x, int y) {
    if (!s || !s->dragging) return -1;
    int coord = s->orientation == 0 ? x : y;
    int new_pos = coord - s->drag_offset;
    if (new_pos < s->min_pos) new_pos = s->min_pos;
    if (new_pos > s->max_pos) new_pos = s->max_pos;
    s->position = new_pos;
    return new_pos;
}
void splitbar_end_drag(SplitBar *s) {
    if (s) s->dragging = 0;
}
int splitbar_position(const SplitBar *s) {
    return s ? s->position : -1;
}
