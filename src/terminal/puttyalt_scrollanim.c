/* puttyalt_scrollanim.c - Smooth scroll animation interpolation. */
#include <stdint.h>
typedef struct {
    int from_pos;
    int to_pos;
    int current_pos;
    uint64_t start_ms;
    int duration_ms;
    int active;
} ScrollAnim;
void scrollanim_init(ScrollAnim *s, int duration_ms) {
    if (!s) return;
    s->from_pos = 0;
    s->to_pos = 0;
    s->current_pos = 0;
    s->start_ms = 0;
    s->duration_ms = duration_ms > 0 ? duration_ms : 150;
    s->active = 0;
}
void scrollanim_start(ScrollAnim *s, int from, int to, uint64_t now_ms) {
    if (!s) return;
    s->from_pos = from;
    s->to_pos = to;
    s->current_pos = from;
    s->start_ms = now_ms;
    s->active = (from != to);
}
int scrollanim_tick(ScrollAnim *s, uint64_t now_ms) {
    if (!s || !s->active) return s ? s->current_pos : 0;
    uint64_t elapsed = now_ms - s->start_ms;
    if (elapsed >= (uint64_t)s->duration_ms) {
        s->current_pos = s->to_pos;
        s->active = 0;
        return s->current_pos;
    }
    /* ease-out cubic */
    int t = (int)(elapsed * 1000 / s->duration_ms);
    int eased = 1000 - (1000 - t) * (1000 - t) / 1000 * (1000 - t) / 1000;
    s->current_pos = s->from_pos + (s->to_pos - s->from_pos) * eased / 1000;
    return s->current_pos;
}
int scrollanim_is_active(const ScrollAnim *s) {
    return s ? s->active : -1;
}
