/* puttyalt_panenav.c - Pane focus navigation for PuttyAlt.
 * Tracks the focused split pane and resolves directional/cyclic focus moves.
 */
#include <string.h>
#include <stdio.h>

#define PANENAV_MAX_RECTS 64

/* Directions for panenav_move(). */
typedef enum {
    PANENAV_DIR_LEFT  = 0,
    PANENAV_DIR_RIGHT = 1,
    PANENAV_DIR_UP    = 2,
    PANENAV_DIR_DOWN  = 3
} PaneNavDirection;

/* A single pane's bounding rectangle (left/top inclusive, width/height in px). */
typedef struct {
    int id;
    int x;
    int y;
    int w;
    int h;
} PaneRect;

/* Immutable list of pane rectangles describing the current layout. */
typedef struct {
    PaneRect rects[PANENAV_MAX_RECTS];
    int count;
} PaneRectList;

/* Mutable focus-navigation state. */
typedef struct {
    int focused_id;
    char last_move[64];
} PaneNavState;

void panenav_init(PaneNavState *n, int focused_id)
{
    if (!n) return;
    memset(n, 0, sizeof(*n));
    n->focused_id = focused_id;
    snprintf(n->last_move, sizeof(n->last_move), "init id=%d", focused_id);
}

int panenav_focused(const PaneNavState *n)
{
    if (!n) return -1;
    return n->focused_id;
}

void panenav_set_focus(PaneNavState *n, int pane_id)
{
    if (!n) return;
    n->focused_id = pane_id;
    snprintf(n->last_move, sizeof(n->last_move), "set id=%d", pane_id);
}

/* Find the index of a pane id within the list, or -1 if absent. */
static int panenav_index_of(const PaneRectList *rects, int id)
{
    int i;
    if (!rects) return -1;
    for (i = 0; i < rects->count && i < PANENAV_MAX_RECTS; i++)
        if (rects->rects[i].id == id) return i;
    return -1;
}

int panenav_move(PaneNavState *n, const PaneRectList *rects, int direction)
{
    int cur, i, best = -1;
    long best_metric = 0;
    int cx, cy;
    if (!n || !rects || rects->count <= 0) return -1;
    if (direction < PANENAV_DIR_LEFT || direction > PANENAV_DIR_DOWN) return -1;
    cur = panenav_index_of(rects, n->focused_id);
    if (cur < 0) return -1;
    cx = rects->rects[cur].x + rects->rects[cur].w / 2;
    cy = rects->rects[cur].y + rects->rects[cur].h / 2;
    for (i = 0; i < rects->count && i < PANENAV_MAX_RECTS; i++) {
        int ox, oy;
        long primary, ortho, metric;
        if (i == cur) continue;
        ox = rects->rects[i].x + rects->rects[i].w / 2;
        oy = rects->rects[i].y + rects->rects[i].h / 2;
        if (direction == PANENAV_DIR_LEFT) {
            if (ox >= cx) continue;
            primary = (long)(cx - ox); ortho = (long)(oy - cy);
        } else if (direction == PANENAV_DIR_RIGHT) {
            if (ox <= cx) continue;
            primary = (long)(ox - cx); ortho = (long)(oy - cy);
        } else if (direction == PANENAV_DIR_UP) {
            if (oy >= cy) continue;
            primary = (long)(cy - oy); ortho = (long)(ox - cx);
        } else {
            if (oy <= cy) continue;
            primary = (long)(oy - cy); ortho = (long)(ox - cx);
        }
        if (ortho < 0) ortho = -ortho;
        metric = primary + ortho * 2;
        if (best < 0 || metric < best_metric) {
            best_metric = metric;
            best = i;
        }
    }
    if (best < 0) return -1;
    n->focused_id = rects->rects[best].id;
    snprintf(n->last_move, sizeof(n->last_move),
             "move dir=%d -> id=%d", direction, n->focused_id);
    return 0;
}

int panenav_cycle(PaneNavState *n, const PaneRectList *rects, int forward)
{
    int cur, next, step;
    if (!n || !rects || rects->count <= 0) return -1;
    cur = panenav_index_of(rects, n->focused_id);
    if (cur < 0) return -1;
    step = forward ? 1 : -1;
    next = (cur + step + rects->count) % rects->count;
    n->focused_id = rects->rects[next].id;
    snprintf(n->last_move, sizeof(n->last_move),
             "cycle fwd=%d -> id=%d", forward ? 1 : 0, n->focused_id);
    return 0;
}
