/* puttyalt_reflowsched.c - Lazy reflow scheduler for PuttyAlt.
 * Queues dirty line ranges on resize and reflows only a bounded budget of
 * lines per tick, prioritising the visible viewport so the screen settles
 * first and avoiding a full-scrollback reflow stall.
 * Self-contained C99; standard headers only; static state. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define REFLOWSCHED_MAX_RANGES 64   /* queued dirty line ranges */

typedef struct reflowsched_range {
    int32_t lo, hi;                 /* inclusive line range [lo, hi] */
} reflowsched_range;

typedef struct reflowsched_state {
    reflowsched_range ranges[REFLOWSCHED_MAX_RANGES];
    int32_t  count;                 /* live ranges */
    int32_t  total_lines;           /* scrollback + screen line count */
    int32_t  budget;                /* max lines reflowed per tick */
    int32_t  view_lo, view_hi;      /* visible viewport line span */
    uint64_t reflowed;              /* lifetime lines reflowed */
    uint64_t ticks;                 /* tick invocations */
    uint64_t last_ms;
    int      ready;
} reflowsched_state;

static reflowsched_state reflowsched_g;

int reflowsched_init(int32_t total_lines, int32_t budget) {
    if (total_lines < 0 || budget <= 0) return -1;
    memset(&reflowsched_g, 0, sizeof(reflowsched_g));
    reflowsched_g.total_lines = total_lines;
    reflowsched_g.budget = budget;
    reflowsched_g.view_hi = total_lines > 0 ? total_lines - 1 : 0;
    reflowsched_g.ready = 1;
    return 0;
}

int reflowsched_set_geometry(int32_t total_lines) {
    if (!reflowsched_g.ready || total_lines < 0) return -1;
    reflowsched_g.total_lines = total_lines;
    if (reflowsched_g.view_hi > total_lines - 1)
        reflowsched_g.view_hi = total_lines > 0 ? total_lines - 1 : 0;
    if (reflowsched_g.view_lo > reflowsched_g.view_hi)
        reflowsched_g.view_lo = reflowsched_g.view_hi;
    return 0;
}

int reflowsched_set_viewport(int32_t lo, int32_t hi) {
    if (!reflowsched_g.ready || lo < 0 || hi < lo) return -1;
    if (hi > reflowsched_g.total_lines - 1)
        hi = reflowsched_g.total_lines > 0 ? reflowsched_g.total_lines - 1 : 0;
    if (lo > hi) lo = hi;
    reflowsched_g.view_lo = lo;
    reflowsched_g.view_hi = hi;
    return 0;
}

int reflowsched_mark_dirty(int32_t lo, int32_t hi) {
    if (!reflowsched_g.ready || lo < 0 || hi < lo) return -1;
    if (hi > reflowsched_g.total_lines - 1) hi = reflowsched_g.total_lines - 1;
    if (lo > hi) return 0;                                  /* nothing to do */
    if (reflowsched_g.count >= REFLOWSCHED_MAX_RANGES) {    /* coalesce all */
        for (int32_t i = 1; i < reflowsched_g.count; i++) {
            if (reflowsched_g.ranges[i].lo < reflowsched_g.ranges[0].lo)
                reflowsched_g.ranges[0].lo = reflowsched_g.ranges[i].lo;
            if (reflowsched_g.ranges[i].hi > reflowsched_g.ranges[0].hi)
                reflowsched_g.ranges[0].hi = reflowsched_g.ranges[i].hi;
        }
        reflowsched_g.count = 1;
    }
    reflowsched_g.ranges[reflowsched_g.count].lo = lo;
    reflowsched_g.ranges[reflowsched_g.count].hi = hi;
    reflowsched_g.count++;
    return 0;
}

/* Reflow up to one budget's worth of lines, viewport ranges first. */
int32_t reflowsched_tick(uint64_t now_ms) {
    int32_t remaining, done = 0, pass;
    if (!reflowsched_g.ready) return -1;
    reflowsched_g.ticks++;
    reflowsched_g.last_ms = now_ms;
    remaining = reflowsched_g.budget;
    for (pass = 0; pass < 2 && remaining > 0; pass++) {
        for (int32_t i = 0; i < reflowsched_g.count && remaining > 0; i++) {
            reflowsched_range *r = &reflowsched_g.ranges[i];
            int32_t inview = (r->lo <= reflowsched_g.view_hi &&
                              r->hi >= reflowsched_g.view_lo);
            int32_t span, take;
            if (pass == 0 && !inview) continue;             /* viewport first */
            if (pass == 1 && inview) continue;
            span = r->hi - r->lo + 1;
            take = span < remaining ? span : remaining;
            r->lo += take;
            remaining -= take;
            done += take;
            if (r->lo > r->hi) {                            /* range drained */
                reflowsched_g.ranges[i] =
                    reflowsched_g.ranges[--reflowsched_g.count];
                i--;
            }
        }
    }
    reflowsched_g.reflowed += (uint64_t)done;
    return done;
}

int reflowsched_is_idle(void) {
    if (!reflowsched_g.ready) return -1;
    return reflowsched_g.count == 0 ? 1 : 0;
}

int32_t reflowsched_pending(void) {
    int32_t i, sum = 0;
    if (!reflowsched_g.ready) return -1;
    for (i = 0; i < reflowsched_g.count; i++)
        sum += reflowsched_g.ranges[i].hi - reflowsched_g.ranges[i].lo + 1;
    return sum;
}

int reflowsched_stats(char *buf, size_t len) {
    int n;
    if (!buf || len == 0) return -1;
    n = snprintf(buf, len,
                 "reflowsched: ranges=%ld/%d pending=%ld budget=%ld "
                 "view=[%ld,%ld] reflowed=%llu ticks=%llu last_ms=%llu",
                 (long)reflowsched_g.count, REFLOWSCHED_MAX_RANGES,
                 (long)reflowsched_pending(), (long)reflowsched_g.budget,
                 (long)reflowsched_g.view_lo, (long)reflowsched_g.view_hi,
                 (unsigned long long)reflowsched_g.reflowed,
                 (unsigned long long)reflowsched_g.ticks,
                 (unsigned long long)reflowsched_g.last_ms);
    if (n < 0 || (size_t)n >= len) return -1;
    return n;
}
