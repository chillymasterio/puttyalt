/*
 * puttyalt_d2dbackend.c - Direct2D backend abstraction for PuttyAlt.
 *
 * Provides backend selection and a capability layer that abstracts a
 * Direct2D path versus a software blitter, exposing uniform draw-target
 * descriptors without linking Direct2D directly.
 *
 * Self-contained: no windows.h, no project headers.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* Backend kinds the abstraction can resolve to. */
enum d2d_kind {
    D2D_KIND_NONE = 0,   /* not yet selected */
    D2D_KIND_HW   = 1,   /* Direct2D hardware-accelerated */
    D2D_KIND_SW   = 2    /* software blitter fallback */
};

/* Capability bit flags reported by d2dbackend_caps(). */
enum d2d_caps {
    D2D_CAP_NONE        = 0,
    D2D_CAP_ALPHA       = 1 << 0,
    D2D_CAP_SUBPIXEL    = 1 << 1,
    D2D_CAP_HW_ACCEL    = 1 << 2,
    D2D_CAP_HIDPI       = 1 << 3
};

#define D2D_MAX_TARGETS   8
#define D2D_NAME_LEN      32
#define D2D_MAX_DIM       16384

/* A single draw-target descriptor produced by the backend. */
typedef struct D2DTarget {
    int   width;
    int   height;
    int   kind;            /* one of enum d2d_kind */
    int   stride;          /* bytes per row, RGBA32 */
    char  desc[D2D_NAME_LEN];
} D2DTarget;

/* The backend state object. */
typedef struct D2DBackend {
    int       prefer_hw;
    int       kind;        /* one of enum d2d_kind */
    int       dpi;
    int       caps;        /* OR of enum d2d_caps */
    int       ntargets;
    D2DTarget targets[D2D_MAX_TARGETS];
} D2DBackend;

void d2dbackend_init(D2DBackend *b, int prefer_hw)
{
    if (!b)
        return;
    memset(b, 0, sizeof(*b));
    b->prefer_hw = prefer_hw ? 1 : 0;
    b->kind = D2D_KIND_NONE;
    b->dpi = 96;
    b->caps = D2D_CAP_NONE;
    b->ntargets = 0;
}

int d2dbackend_select(D2DBackend *b, int hw_available, int dpi)
{
    if (!b || dpi <= 0 || dpi > 4096)
        return -1;
    b->dpi = dpi;
    if (b->prefer_hw && hw_available) {
        b->kind = D2D_KIND_HW;
        b->caps = D2D_CAP_ALPHA | D2D_CAP_SUBPIXEL | D2D_CAP_HW_ACCEL;
    } else {
        b->kind = D2D_KIND_SW;
        b->caps = D2D_CAP_ALPHA;
    }
    if (dpi > 96)
        b->caps |= D2D_CAP_HIDPI;
    return 0;
}

int d2dbackend_create_target(D2DBackend *b, int w, int h, D2DTarget *out)
{
    D2DTarget *slot;
    if (!b || !out || w <= 0 || h <= 0)
        return -1;
    if (w > D2D_MAX_DIM || h > D2D_MAX_DIM)
        return -1;
    if (b->kind == D2D_KIND_NONE)
        return -1;
    if (b->ntargets >= D2D_MAX_TARGETS)
        return -1;

    slot = &b->targets[b->ntargets];
    slot->width = w;
    slot->height = h;
    slot->kind = b->kind;
    slot->stride = w * 4;
    snprintf(slot->desc, sizeof(slot->desc), "%s %dx%d@%d",
             (b->kind == D2D_KIND_HW) ? "hw" : "sw", w, h, b->dpi);
    b->ntargets++;
    *out = *slot;
    return 0;
}

int d2dbackend_caps(const D2DBackend *b)
{
    if (!b)
        return -1;
    return b->caps;
}

const char *d2dbackend_name(const D2DBackend *b)
{
    if (!b)
        return "invalid";
    switch (b->kind) {
        case D2D_KIND_HW:   return "direct2d-hw";
        case D2D_KIND_SW:   return "software-blit";
        default:            return "unselected";
    }
}
