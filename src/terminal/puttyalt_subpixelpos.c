/* puttyalt_subpixelpos.c - Subpixel text positioning for PuttyAlt.
 * Accumulates fractional glyph advances across a line, quantizes each origin
 * to one of N subpixel phases (RGB LCD thirds by default), and emits integer
 * pixel x plus phase index while tracking residual error so the line stays
 * advance-correct. Self-contained C99, no project headers. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SUBPIXELPOS_SCALE     256   /* fraction units per pixel */
#define SUBPIXELPOS_MAX_PHASE 16    /* hard cap on phases */

typedef struct subpixelpos_state {
    int32_t phases;     /* active subpixel phases (1..MAX_PHASE) */
    int64_t accum;      /* accumulated advance in SCALE units */
    int64_t error;      /* residual rounding error in SCALE units */
    int32_t glyphs;     /* advances applied since reset */
    int32_t last_x;     /* last emitted integer pixel x */
    int32_t last_phase; /* last emitted phase index */
} subpixelpos_state;

int subpixelpos_init(subpixelpos_state *s)
{
    if (!s)
        return -1;
    memset(s, 0, sizeof(*s));
    s->phases = 3;
    return 0;
}

int subpixelpos_set_phases(subpixelpos_state *s, int phases)
{
    if (!s || phases < 1 || phases > SUBPIXELPOS_MAX_PHASE)
        return -1;
    s->phases = (int32_t)phases;
    return 0;
}

int subpixelpos_reset(subpixelpos_state *s)
{
    if (!s)
        return -1;
    s->accum = s->error = 0;
    s->glyphs = s->last_x = s->last_phase = 0;
    return 0;
}

int subpixelpos_advance(subpixelpos_state *s, int advance_units)
{
    if (!s || advance_units < 0)
        return -1;
    s->accum += (int64_t)advance_units;
    if (s->glyphs < INT32_MAX)
        s->glyphs++;
    return 0;
}

/* Quantize the current position to the nearest phase boundary, folding the
 * rounding residual into tracked error. Emits x and phase. Returns 0 / -1. */
int subpixelpos_origin_for(subpixelpos_state *s, int *out_x, int *out_phase)
{
    int64_t pos, total, x, phase;
    if (!s || !out_x || !out_phase || s->phases < 1)
        return -1;
    pos = s->accum + s->error;
    /* position in phase-units across the whole pixel grid */
    total = (pos * s->phases + SUBPIXELPOS_SCALE / 2) / SUBPIXELPOS_SCALE;
    s->error = pos - (total * SUBPIXELPOS_SCALE) / s->phases;
    x = total / s->phases;
    phase = total % s->phases;
    if (phase < 0) {
        phase += s->phases;
        x -= 1;
    }
    s->last_x = (int32_t)x;
    s->last_phase = (int32_t)phase;
    *out_x = (int)x;
    *out_phase = (int)phase;
    return 0;
}

int subpixelpos_phase_index(const subpixelpos_state *s, int pos_units)
{
    int64_t total, phase;
    if (!s || s->phases < 1)
        return -1;
    total = ((int64_t)pos_units * s->phases + SUBPIXELPOS_SCALE / 2)
            / SUBPIXELPOS_SCALE;
    phase = total % s->phases;
    if (phase < 0)
        phase += s->phases;
    return (int)phase;
}

/* Describe current state into buf. Returns bytes written, or -1 on error. */
int subpixelpos_describe(const subpixelpos_state *s, char *buf, size_t len)
{
    int n;
    if (!s || !buf || len == 0)
        return -1;
    n = snprintf(buf, len,
                 "subpixelpos: phases=%d glyphs=%d accum=%ld err=%ld "
                 "last_x=%d last_phase=%d",
                 (int)s->phases, (int)s->glyphs, (long)s->accum,
                 (long)s->error, (int)s->last_x, (int)s->last_phase);
    if (n < 0 || (size_t)n >= len)
        return -1;
    return n;
}
