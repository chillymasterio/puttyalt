/* puttyalt_damageregion.c - Damage-region compositor for PuttyAlt.
 * Accumulates dirty rects, coalesces overlapping/adjacent regions, clips to
 * the framebuffer extent, emits a minimized rect set, and falls back to full
 * damage on overflow. Self-contained C99; standard headers only. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DAMAGEREGION_MAX_RECTS 64

typedef struct damageregion_rect {
    int32_t x, y, w, h;
} damageregion_rect;

typedef struct damageregion_compositor {
    damageregion_rect rects[DAMAGEREGION_MAX_RECTS];
    int32_t count;
    int32_t fb_w, fb_h;     /* framebuffer extent */
    int      overflowed;     /* set when capped -> full damage */
    uint64_t last_ms;
} damageregion_compositor;

/* True if a and b overlap or are edge-adjacent (mergeable). */
static int damageregion_overlap_adj(const damageregion_rect *a,
                                    const damageregion_rect *b) {
    int32_t ar = a->x + a->w, ab = a->y + a->h;
    int32_t br = b->x + b->w, bb = b->y + b->h;
    if (a->x > br || b->x > ar) return 0;
    if (a->y > bb || b->y > ab) return 0;
    return 1;
}

static void damageregion_union(damageregion_rect *a,
                               const damageregion_rect *b) {
    int32_t x0 = a->x < b->x ? a->x : b->x;
    int32_t y0 = a->y < b->y ? a->y : b->y;
    int32_t ar = a->x + a->w, br = b->x + b->w;
    int32_t ab = a->y + a->h, bb = b->y + b->h;
    int32_t x1 = ar > br ? ar : br;
    int32_t y1 = ab > bb ? ab : bb;
    a->x = x0; a->y = y0; a->w = x1 - x0; a->h = y1 - y0;
}

int damageregion_init(damageregion_compositor *c, int32_t fb_w, int32_t fb_h) {
    if (!c || fb_w <= 0 || fb_h <= 0) return -1;
    memset(c, 0, sizeof(*c));
    c->fb_w = fb_w;
    c->fb_h = fb_h;
    return 0;
}

void damageregion_reset(damageregion_compositor *c) {
    if (!c) return;
    c->count = 0;
    c->overflowed = 0;
}

int damageregion_full_damage(damageregion_compositor *c, uint64_t now_ms) {
    if (!c) return -1;
    c->rects[0].x = 0; c->rects[0].y = 0;
    c->rects[0].w = c->fb_w; c->rects[0].h = c->fb_h;
    c->count = 1;
    c->overflowed = 1;
    c->last_ms = now_ms;
    return 0;
}

int damageregion_add(damageregion_compositor *c, int32_t x, int32_t y,
                     int32_t w, int32_t h, uint64_t now_ms) {
    if (!c) return -1;
    c->last_ms = now_ms;
    if (w <= 0 || h <= 0) return 0;
    /* empty rect: nothing to do */
    if (c->overflowed) return 0;
    /* already full damage */
    if (c->count >= DAMAGEREGION_MAX_RECTS)
        return damageregion_full_damage(c, now_ms);
    c->rects[c->count].x = x; c->rects[c->count].y = y;
    c->rects[c->count].w = w; c->rects[c->count].h = h;
    c->count++;
    return 0;
}

int damageregion_coalesce(damageregion_compositor *c) {
    int merged = 1;
    if (!c) return -1;
    while (merged) {
        merged = 0;
        for (int32_t i = 0; i < c->count; i++) {
            for (int32_t j = i + 1; j < c->count; j++) {
                if (damageregion_overlap_adj(&c->rects[i], &c->rects[j])) {
                    damageregion_union(&c->rects[i], &c->rects[j]);
                    c->rects[j] = c->rects[c->count - 1];
                    c->count--;
                    merged = 1;
                    j--;
                }
            }
        }
    }
    return 0;
}

int damageregion_clip(damageregion_compositor *c) {
    int32_t out = 0;
    if (!c) return -1;
    for (int32_t i = 0; i < c->count; i++) {
        damageregion_rect r = c->rects[i];
        int32_t x1 = r.x + r.w, y1 = r.y + r.h;
        if (r.x < 0) r.x = 0;
        if (r.y < 0) r.y = 0;
        if (x1 > c->fb_w) x1 = c->fb_w;
        if (y1 > c->fb_h) y1 = c->fb_h;
        if (x1 <= r.x || y1 <= r.y) continue;
        /* fully outside */
        r.w = x1 - r.x; r.h = y1 - r.y;
        c->rects[out++] = r;
    }
    c->count = out;
    return 0;
}

int32_t damageregion_count(const damageregion_compositor *c) {
    return c ? c->count : -1;
}

int damageregion_get(const damageregion_compositor *c, int32_t idx,
                     damageregion_rect *out) {
    if (!c || !out || idx < 0 || idx >= c->count) return -1;
    *out = c->rects[idx];
    return 0;
}

int damageregion_describe(const damageregion_compositor *c,
                          char *buf, size_t len) {
    int n;
    if (!c || !buf || len == 0) return -1;
    n = snprintf(buf, len,
                 "damageregion: rects=%ld/%d fb=%ldx%ld%s last_ms=%llu",
                 (long)c->count, DAMAGEREGION_MAX_RECTS,
                 (long)c->fb_w, (long)c->fb_h,
                 c->overflowed ? " [FULL]" : "",
                 (unsigned long long)c->last_ms);
    if (n < 0 || (size_t)n >= len) return -1;
    return n;
}
