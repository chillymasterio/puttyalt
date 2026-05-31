/* puttyalt_rendercmdq.c - bounded SPSC render command ring buffer. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RENDERCMDQ_CAP 64u

enum rendercmdq_kind {
    RENDERCMDQ_DRAW_TEXT = 0,
    RENDERCMDQ_FILL_RECT,
    RENDERCMDQ_BLIT_GLYPH,
    RENDERCMDQ_PRESENT,
    RENDERCMDQ_RESIZE
};

typedef struct rendercmdq_cmd {
    uint32_t kind;          /* enum rendercmdq_kind */
    int32_t  x, y, w, h;    /* small fixed payload */
    uint32_t glyph;         /* codepoint / glyph id */
    uint32_t color;         /* packed RGBA */
    uint64_t seq_ms;        /* caller-supplied timestamp */
} rendercmdq_cmd;

typedef struct rendercmdq {
    rendercmdq_cmd slots[RENDERCMDQ_CAP];
    uint32_t head;          /* consumer index */
    uint32_t tail;          /* producer index */
    uint32_t count;         /* live entries */
    uint64_t overflow;      /* dropped pushes */
    uint64_t pushed;        /* total accepted */
    uint64_t popped;        /* total consumed */
} rendercmdq;

int rendercmdq_init(rendercmdq *q)
{
    if (!q)
        return -1;
    memset(q, 0, sizeof(*q));
    return 0;
}

int rendercmdq_reset(rendercmdq *q)
{
    if (!q)
        return -1;
    q->head = q->tail = q->count = 0u;
    q->overflow = q->pushed = q->popped = 0u;
    memset(q->slots, 0, sizeof(q->slots));
    return 0;
}

int rendercmdq_is_full(const rendercmdq *q)
{
    return (q && q->count >= RENDERCMDQ_CAP) ? 1 : 0;
}

int rendercmdq_is_empty(const rendercmdq *q)
{
    return (!q || q->count == 0u) ? 1 : 0;
}

uint32_t rendercmdq_pending(const rendercmdq *q)
{
    return q ? q->count : 0u;
}

int rendercmdq_push(rendercmdq *q, const rendercmdq_cmd *cmd, uint64_t now_ms)
{
    rendercmdq_cmd *dst;
    if (!q || !cmd)
        return -1;
    if (q->count >= RENDERCMDQ_CAP) {
        q->overflow++;
        return -1;
    }
    dst = &q->slots[q->tail];
    *dst = *cmd;
    dst->seq_ms = now_ms;
    q->tail = (q->tail + 1u) % RENDERCMDQ_CAP;
    q->count++;
    q->pushed++;
    return 0;
}

int rendercmdq_pop(rendercmdq *q, rendercmdq_cmd *out)
{
    if (!q || !out)
        return -1;
    if (q->count == 0u)
        return -1;
    *out = q->slots[q->head];
    q->head = (q->head + 1u) % RENDERCMDQ_CAP;
    q->count--;
    q->popped++;
    return 0;
}

int rendercmdq_describe(const rendercmdq *q, char *buf, size_t len)
{
    int n;
    if (!q || !buf || len == 0u)
        return -1;
    n = snprintf(buf, len,
                 "rendercmdq: pending=%u/%u pushed=%llu popped=%llu overflow=%llu",
                 q->count, (unsigned)RENDERCMDQ_CAP,
                 (unsigned long long)q->pushed,
                 (unsigned long long)q->popped,
                 (unsigned long long)q->overflow);
    if (n < 0 || (size_t)n >= len)
        return -1;
    return n;
}
