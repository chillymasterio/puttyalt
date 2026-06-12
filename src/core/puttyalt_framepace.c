/*
 * puttyalt_framepace.c - Frame pacing for repaint scheduling.
 *
 * Paces repaint scheduling toward a target frame rate. Decides whether
 * the current tick should render or be coalesced, based on elapsed time
 * and an accumulated frame-time budget. Caller supplies a monotonic
 * millisecond clock (now_ms); this module performs no time-of-day calls.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FP_MIN_FPS        1
#define FP_MAX_FPS        480
#define FP_DEFAULT_FPS    60
#define FP_HISTORY        16    /* recent inter-frame intervals (ms) */

typedef struct FramePacer {
    int target_fps;                  /* clamped target frame rate */
    uint32_t frame_ms;               /* nominal ms per frame */
    uint32_t budget_ms;              /* accumulated render budget */
    uint32_t last_render_ms;         /* timestamp of last render */
    int have_last;                   /* 0 until first render recorded */
    uint32_t history[FP_HISTORY];    /* ring of recent frame intervals */
    int hist_count;                  /* valid entries (<= FP_HISTORY) */
    int hist_head;                   /* next write index into history */
    char label[32];                  /* formatted "fps=N" for diagnostics */
} FramePacer;

static int fp_clamp_fps(int fps)
{
    if (fps < FP_MIN_FPS) return FP_MIN_FPS;
    if (fps > FP_MAX_FPS) return FP_MAX_FPS;
    return fps;
}

static void fp_apply_target(FramePacer *p, int target_fps)
{
    p->target_fps = fp_clamp_fps(target_fps);
    p->frame_ms = (uint32_t)(1000 / p->target_fps);
    if (p->frame_ms == 0) p->frame_ms = 1;
    snprintf(p->label, sizeof(p->label), "fps=%d", p->target_fps);
}

void framepace_init(FramePacer *p, int target_fps)
{
    if (!p) return;
    memset(p, 0, sizeof(*p));
    fp_apply_target(p, target_fps);
    p->budget_ms = p->frame_ms;   /* allow an immediate first render */
    p->have_last = 0;
    p->hist_count = 0;
    p->hist_head = 0;
}

void framepace_set_target(FramePacer *p, int target_fps)
{
    if (!p) return;
    fp_apply_target(p, target_fps);
    if (p->budget_ms > p->frame_ms * 2)
        p->budget_ms = p->frame_ms * 2;   /* avoid burst after speed-up */
}

int framepace_should_render(FramePacer *p, unsigned int now_ms)
{
    uint32_t elapsed;
    if (!p) return -1;
    if (!p->have_last) return 1;
    /* first frame always renders */
    elapsed = (uint32_t)now_ms - p->last_render_ms;
    /* Accumulate elapsed time into the budget, capped to avoid runaway. */
    if (p->budget_ms + elapsed < p->budget_ms)
        p->budget_ms = p->frame_ms * 4;     /* overflow guard */
    else
        p->budget_ms += elapsed;
    if (p->budget_ms > p->frame_ms * 4)
        p->budget_ms = p->frame_ms * 4;
    return (p->budget_ms >= p->frame_ms) ? 1 : 0;
}

void framepace_mark_rendered(FramePacer *p, unsigned int now_ms)
{
    uint32_t interval;
    if (!p) return;
    if (p->have_last) {
        interval = (uint32_t)now_ms - p->last_render_ms;
        p->history[p->hist_head] = interval;
        p->hist_head = (p->hist_head + 1) % FP_HISTORY;
        if (p->hist_count < FP_HISTORY) p->hist_count++;
    }
    /* Consume one frame's worth of budget; keep remainder for coalescing. */
    if (p->budget_ms >= p->frame_ms)
        p->budget_ms -= p->frame_ms;
    else
        p->budget_ms = 0;
    p->last_render_ms = (uint32_t)now_ms;
    p->have_last = 1;
}

int framepace_avg_fps(const FramePacer *p)
{
    uint64_t sum = 0;
    int i;
    uint32_t avg_ms;
    if (!p || p->hist_count <= 0) return -1;
    for (i = 0; i < p->hist_count; i++)
        sum += p->history[i];
    avg_ms = (uint32_t)(sum / (uint64_t)p->hist_count);
    if (avg_ms == 0) return FP_MAX_FPS;
    return (int)(1000 / avg_ms);
}
