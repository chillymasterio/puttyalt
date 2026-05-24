#include "puttyalt_transitions.h"
#include <math.h>

void transition_init(Transition *t, float initial, float speed)
{
    t->current = initial;
    t->target = initial;
    t->speed = speed;
}

void transition_set(Transition *t, float target)
{
    t->target = target;
}

float transition_update(Transition *t, float dt)
{
    if (t->current < t->target) {
        t->current += t->speed * dt;
        if (t->current > t->target) t->current = t->target;
    } else if (t->current > t->target) {
        t->current -= t->speed * dt;
        if (t->current < t->target) t->current = t->target;
    }
    return t->current;
}

int transition_done(const Transition *t)
{
    return fabsf(t->current - t->target) < 0.001f;
}

float ease_linear(float t) { return t; }

float ease_in_quad(float t) { return t * t; }

float ease_out_quad(float t) { return t * (2.0f - t); }

float ease_in_out_cubic(float t)
{
    if (t < 0.5f)
        return 4.0f * t * t * t;
    float f = (2.0f * t) - 2.0f;
    return 0.5f * f * f * f + 1.0f;
}

unsigned int color_lerp(unsigned int from, unsigned int to, float t)
{
    if (t <= 0.0f) return from;
    if (t >= 1.0f) return to;

    int r1 = (from >> 16) & 0xFF, g1 = (from >> 8) & 0xFF, b1 = from & 0xFF;
    int r2 = (to >> 16) & 0xFF, g2 = (to >> 8) & 0xFF, b2 = to & 0xFF;

    int r = r1 + (int)((r2 - r1) * t);
    int g = g1 + (int)((g2 - g1) * t);
    int b = b1 + (int)((b2 - b1) * t);

    return ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
}

unsigned int color_alpha_blend(unsigned int fg, unsigned int bg, float alpha)
{
    return color_lerp(bg, fg, alpha);
}
