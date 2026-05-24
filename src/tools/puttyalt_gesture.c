/*
 * puttyalt_gesture.c: Mouse gesture recognition.
 */

#include <string.h>
#include <stdlib.h>
#include "puttyalt_gesture.h"

void gesture_init(GestureRecognizer *gr)
{
    memset(gr, 0, sizeof(*gr));
    /* Default bindings */
    gr->bindings[GESTURE_RIGHT]      = GACT_NEXT_TAB;
    gr->bindings[GESTURE_LEFT]       = GACT_PREV_TAB;
    gr->bindings[GESTURE_DOWN_RIGHT] = GACT_NEW_TAB;
    gr->bindings[GESTURE_LEFT_RIGHT] = GACT_CLOSE_TAB;
    gr->bindings[GESTURE_UP_DOWN]    = GACT_PASTE;
    gr->bindings[GESTURE_UP]         = GACT_SCROLL_TOP;
    gr->bindings[GESTURE_DOWN]       = GACT_SCROLL_BOTTOM;
    gr->bindings[GESTURE_UP_LEFT]    = GACT_RECONNECT;
}

void gesture_start(GestureRecognizer *gr, int x, int y)
{
    gr->num_points = 0;
    gr->recording = 1;
    gr->points[0].x = x;
    gr->points[0].y = y;
    gr->num_points = 1;
}

void gesture_move(GestureRecognizer *gr, int x, int y)
{
    if (!gr->recording)
        return;
    if (gr->num_points >= GESTURE_MAX_POINTS)
        return;
    gr->points[gr->num_points].x = x;
    gr->points[gr->num_points].y = y;
    gr->num_points++;
}

GestureType gesture_end(GestureRecognizer *gr)
{
    gr->recording = 0;

    if (gr->num_points < 3)
        return GESTURE_NONE;

    int x0 = gr->points[0].x;
    int y0 = gr->points[0].y;
    int x1 = gr->points[gr->num_points - 1].x;
    int y1 = gr->points[gr->num_points - 1].y;

    int dx = x1 - x0;
    int dy = y1 - y0;
    int adx = abs(dx);
    int ady = abs(dy);

    /* Check minimum distance */
    if (adx < GESTURE_MIN_DISTANCE && ady < GESTURE_MIN_DISTANCE)
        return GESTURE_NONE;

    /* Find midpoint for compound gestures */
    int mid = gr->num_points / 2;
    int mx = gr->points[mid].x;
    int my = gr->points[mid].y;
    int dx1 = mx - x0, dy1 = my - y0;
    int dx2 = x1 - mx, dy2 = y1 - my;

    /* Check for compound gesture: direction changes */
    if (abs(dx1) > GESTURE_MIN_DISTANCE && abs(dx2) > GESTURE_MIN_DISTANCE) {
        if ((dx1 > 0) != (dx2 > 0))
            return GESTURE_LEFT_RIGHT;
    }
    if (abs(dy1) > GESTURE_MIN_DISTANCE && abs(dy2) > GESTURE_MIN_DISTANCE) {
        if ((dy1 > 0) != (dy2 > 0))
            return GESTURE_UP_DOWN;
    }

    /* Simple directional gesture */
    if (adx > ady) {
        if (ady > adx / 3) {
            /* diagonal */
            if (dx > 0 && dy > 0) return GESTURE_DOWN_RIGHT;
            if (dx < 0 && dy < 0) return GESTURE_UP_LEFT;
        }
        return dx > 0 ? GESTURE_RIGHT : GESTURE_LEFT;
    } else {
        return dy > 0 ? GESTURE_DOWN : GESTURE_UP;
    }
}

GestureAction gesture_get_action(const GestureRecognizer *gr, GestureType g)
{
    if (g <= GESTURE_NONE || g > GESTURE_UP_LEFT)
        return GACT_NONE;
    return gr->bindings[g];
}

void gesture_bind(GestureRecognizer *gr, GestureType g, GestureAction act)
{
    if (g > GESTURE_NONE && g <= GESTURE_UP_LEFT)
        gr->bindings[g] = act;
}
