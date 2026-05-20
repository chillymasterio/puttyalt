#ifndef PUTTYALT_TRANSITIONS_H
#define PUTTYALT_TRANSITIONS_H

typedef struct {
    float current;
    float target;
    float speed;  /* units per frame */
} Transition;

void transition_init(Transition *t, float initial, float speed);
void transition_set(Transition *t, float target);
float transition_update(Transition *t, float dt);
int transition_done(const Transition *t);

/* Easing functions */
float ease_linear(float t);
float ease_in_quad(float t);
float ease_out_quad(float t);
float ease_in_out_cubic(float t);

/* Color interpolation */
unsigned int color_lerp(unsigned int from, unsigned int to, float t);
unsigned int color_alpha_blend(unsigned int fg, unsigned int bg, float alpha);

#endif
