/*
 * puttyalt_gesture.h: Mouse gesture recognition.
 *
 * Recognizes directional mouse gestures (right-button drag)
 * and maps them to configurable actions.
 */

#ifndef PUTTYALT_GESTURE_H
#define PUTTYALT_GESTURE_H

#define GESTURE_MAX_POINTS   256
#define GESTURE_MIN_DISTANCE 30   /* pixels */

typedef enum {
    GESTURE_NONE = 0,
    GESTURE_LEFT,
    GESTURE_RIGHT,
    GESTURE_UP,
    GESTURE_DOWN,
    GESTURE_LEFT_RIGHT,   /* back-forward: close tab */
    GESTURE_UP_DOWN,      /* flick: paste */
    GESTURE_DOWN_RIGHT,   /* new tab */
    GESTURE_UP_LEFT       /* reopen closed tab */
} GestureType;

typedef enum {
    GACT_NONE = 0,
    GACT_NEW_TAB,
    GACT_CLOSE_TAB,
    GACT_NEXT_TAB,
    GACT_PREV_TAB,
    GACT_PASTE,
    GACT_COPY,
    GACT_RECONNECT,
    GACT_SCROLL_TOP,
    GACT_SCROLL_BOTTOM
} GestureAction;

typedef struct GesturePoint {
    int x, y;
} GesturePoint;

typedef struct GestureRecognizer {
    GesturePoint points[GESTURE_MAX_POINTS];
    int num_points;
    int recording;
    GestureAction bindings[9]; /* indexed by GestureType */
} GestureRecognizer;

void gesture_init(GestureRecognizer *gr);
void gesture_start(GestureRecognizer *gr, int x, int y);
void gesture_move(GestureRecognizer *gr, int x, int y);
GestureType gesture_end(GestureRecognizer *gr);
GestureAction gesture_get_action(const GestureRecognizer *gr, GestureType g);
void gesture_bind(GestureRecognizer *gr, GestureType g, GestureAction act);

#endif /* PUTTYALT_GESTURE_H */
