#ifndef PUTTYALT_FOCUSRING_H
#define PUTTYALT_FOCUSRING_H

typedef enum {
    FOCUS_NONE,
    FOCUS_TERMINAL,
    FOCUS_SIDEBAR,
    FOCUS_TABBAR,
    FOCUS_CMDPAL
} FocusTarget;

typedef struct {
    FocusTarget current;
    FocusTarget previous;
    int ring_visible;
    int ring_width;
    unsigned int ring_color;
    int animate;
    float opacity;
} FocusRing;

void focus_ring_init(FocusRing *fr);
void focus_ring_set(FocusRing *fr, FocusTarget target);
void focus_ring_cycle(FocusRing *fr);
FocusTarget focus_ring_get(const FocusRing *fr);
int  focus_ring_should_draw(const FocusRing *fr, FocusTarget panel);

#endif
