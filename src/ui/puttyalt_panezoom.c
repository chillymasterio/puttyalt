/* puttyalt_panezoom.c - Pane zoom toggle for PuttyAlt (MinGW/Windows target).
 *
 * Toggles a single pane to fill its container and back, remembering each
 * pane's pre-zoom rectangle so the original layout can be restored.
 *
 * Build: x86_64-w64-mingw32-gcc -c -Wall -std=c99 puttyalt_panezoom.c
 */

#include <string.h>
#include <stdio.h>

#define PANEZOOM_MAX 64

typedef struct {
    int  pane_id;   /* identifier of the saved pane (>= 0 when used)   */
    int  x, y;      /* pre-zoom top-left position                       */
    int  w, h;      /* pre-zoom size                                    */
    int  used;      /* slot occupied flag                               */
} PaneZoomEntry;

typedef struct {
    int           container_w;
    int           container_h;
    int           zoomed_pane;          /* currently zoomed id, -1 if none */
    int           count;                /* number of used entries          */
    PaneZoomEntry entries[PANEZOOM_MAX];
} PaneZoomState;

/* Locate the slot holding pane_id, or -1 if not stored. */
static int panezoom_find(const PaneZoomState *z, int pane_id) {
    int i;
    if (!z) return -1;
    for (i = 0; i < PANEZOOM_MAX; i++) {
        if (z->entries[i].used && z->entries[i].pane_id == pane_id)
            return i;
    }
    return -1;
}

void panezoom_init(PaneZoomState *z, int container_w, int container_h) {
    if (!z) return;
    memset(z, 0, sizeof(*z));
    z->container_w = (container_w > 0) ? container_w : 0;
    z->container_h = (container_h > 0) ? container_h : 0;
    z->zoomed_pane = -1;
    z->count = 0;
}

/* Toggle zoom on pane_id. (x,y,w,h) is its current rect, saved on zoom-in.
 * Returns 1 if the pane is now zoomed, 0 if now restored, -1 on error. */
int panezoom_toggle(PaneZoomState *z, int pane_id, int x, int y, int w, int h) {
    int slot;
    if (!z || pane_id < 0 || w < 0 || h < 0) return -1;

    if (z->zoomed_pane == pane_id) {
        /* Already zoomed: restore by clearing the active flag. */
        slot = panezoom_find(z, pane_id);
        if (slot >= 0) {
            z->entries[slot].used = 0;
            if (z->count > 0) z->count--;
        }
        z->zoomed_pane = -1;
        return 0;
    }

    /* Only one pane may be zoomed at a time. */
    if (z->zoomed_pane != -1) return -1;

    slot = panezoom_find(z, pane_id);
    if (slot < 0) {
        int i;
        for (i = 0; i < PANEZOOM_MAX; i++) {
            if (!z->entries[i].used) { slot = i; break; }
        }
        if (slot < 0) return -1;        /* storage full */
        z->count++;
    }

    z->entries[slot].pane_id = pane_id;
    z->entries[slot].x = x;
    z->entries[slot].y = y;
    z->entries[slot].w = w;
    z->entries[slot].h = h;
    z->entries[slot].used = 1;
    z->zoomed_pane = pane_id;
    return 1;
}

int panezoom_is_zoomed(const PaneZoomState *z, int pane_id) {
    if (!z || pane_id < 0) return 0;
    return (z->zoomed_pane == pane_id) ? 1 : 0;
}

/* Fetch a pane's rect. Zoomed panes report the full container rect;
 * stored-but-not-zoomed panes report their saved rect. 0 ok, -1 error. */
int panezoom_get_rect(const PaneZoomState *z, int pane_id,
                      int *x, int *y, int *w, int *h) {
    int slot;
    if (!z || pane_id < 0) return -1;

    if (z->zoomed_pane == pane_id) {
        if (x) *x = 0;
        if (y) *y = 0;
        if (w) *w = z->container_w;
        if (h) *h = z->container_h;
        return 0;
    }

    slot = panezoom_find(z, pane_id);
    if (slot < 0) return -1;
    if (x) *x = z->entries[slot].x;
    if (y) *y = z->entries[slot].y;
    if (w) *w = z->entries[slot].w;
    if (h) *h = z->entries[slot].h;
    return 0;
}

void panezoom_clear(PaneZoomState *z) {
    int cw, ch;
    if (!z) return;
    cw = z->container_w;
    ch = z->container_h;
    memset(z->entries, 0, sizeof(z->entries));
    z->count = 0;
    z->zoomed_pane = -1;
    z->container_w = cw;
    z->container_h = ch;
}
