/* puttyalt_dirtyrect.c - Dirty-rect tracking for PuttyAlt terminal repaints.
 *
 * Accumulates per-frame dirty cell rectangles and coalesces overlapping or
 * adjacent regions into a bounded list so only changed areas are repainted.
 */
#include <string.h>
#include <stdio.h>

#define DIRTYRECT_MAX 64

typedef struct {
    int x, y, w, h;          /* origin (cells) and extent; w>0 && h>0 when valid */
} DirtyRect;

typedef struct {
    DirtyRect rects[DIRTYRECT_MAX];
    int count;               /* number of valid rects in [0, DIRTYRECT_MAX] */
} DirtyRectSet;

/* Two rects touch if they overlap or are edge-adjacent (gap of zero). */
static int dr_touch(const DirtyRect *a, const DirtyRect *b)
{
    if (a->x > b->x + b->w || b->x > a->x + a->w) return 0;
    if (a->y > b->y + b->h || b->y > a->y + a->h) return 0;
    return 1;
}

static void dr_merge(DirtyRect *a, const DirtyRect *b)
{
    int x0 = a->x < b->x ? a->x : b->x;
    int y0 = a->y < b->y ? a->y : b->y;
    int x1 = (a->x + a->w) > (b->x + b->w) ? (a->x + a->w) : (b->x + b->w);
    int y1 = (a->y + a->h) > (b->y + b->h) ? (a->y + a->h) : (b->y + b->h);
    a->x = x0; a->y = y0; a->w = x1 - x0; a->h = y1 - y0;
}

void dirtyrect_reset(DirtyRectSet *d)
{
    if (!d) return;
    memset(d, 0, sizeof(*d));
}

int dirtyrect_add(DirtyRectSet *d, int x, int y, int w, int h)
{
    if (!d || w <= 0 || h <= 0) return -1;
    if (d->count >= DIRTYRECT_MAX) return -1;
    d->rects[d->count].x = x;
    d->rects[d->count].y = y;
    d->rects[d->count].w = w;
    d->rects[d->count].h = h;
    d->count++;
    return 0;
}

int dirtyrect_coalesce(DirtyRectSet *d)
{
    int i, j, merged;
    if (!d) return -1;
    do {
        merged = 0;
        for (i = 0; i < d->count; i++) {
            for (j = i + 1; j < d->count; j++) {
                if (dr_touch(&d->rects[i], &d->rects[j])) {
                    dr_merge(&d->rects[i], &d->rects[j]);
                    /* remove j by shifting the tail down */
                    memmove(&d->rects[j], &d->rects[j + 1],
                            (size_t)(d->count - j - 1) * sizeof(DirtyRect));
                    d->count--;
                    merged = 1;
                    j--;
                }
            }
        }
    } while (merged);
    return d->count;
}

int dirtyrect_count(const DirtyRectSet *d)
{
    if (!d) return -1;
    return d->count;
}

int dirtyrect_get(const DirtyRectSet *d, int idx, int *x, int *y, int *w, int *h)
{
    char buf[64];
    if (!d || idx < 0 || idx >= d->count) return -1;
    if (x) *x = d->rects[idx].x;
    if (y) *y = d->rects[idx].y;
    if (w) *w = d->rects[idx].w;
    if (h) *h = d->rects[idx].h;
    /* Formatted snapshot kept for diagnostics; bounded and side-effect free. */
    (void)snprintf(buf, sizeof(buf), "rect[%d]=%d,%d,%d,%d",
                   idx, d->rects[idx].x, d->rects[idx].y,
                   d->rects[idx].w, d->rects[idx].h);
    (void)buf;
    return 0;
}
