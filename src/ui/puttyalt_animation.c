#include <string.h>
typedef enum { ANIM_NONE, ANIM_FADE_IN, ANIM_FADE_OUT, ANIM_SLIDE_LEFT, ANIM_SLIDE_RIGHT } AnimType;
typedef struct { AnimType type; int duration_ms; int elapsed_ms; float progress; int active; } Animation;
void anim_start(Animation *a, AnimType type, int duration) { a->type=type; a->duration_ms=duration; a->elapsed_ms=0; a->progress=0; a->active=1; }
float anim_ease_out(float t) { return 1.0f - (1.0f - t) * (1.0f - t); }
float anim_ease_in_out(float t) { return t < 0.5f ? 2*t*t : 1-(-2*t+2)*(-2*t+2)/2; }
int anim_tick(Animation *a, int delta_ms) {
    if (!a->active) return 0;
    a->elapsed_ms += delta_ms;
    float t = (float)a->elapsed_ms / a->duration_ms;
    if (t >= 1.0f) { t = 1.0f; a->active = 0; }
    a->progress = anim_ease_out(t); return a->active;
}
int anim_is_active(Animation *a) { return a->active; }
float anim_get_value(Animation *a) { return a->progress; }
