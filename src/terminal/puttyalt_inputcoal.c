/*
 * puttyalt_inputcoal.c - Input coalescing buffer for PuttyAlt.
 *
 * Keystrokes and paste fragments that arrive in quick succession are merged
 * into a single contiguous buffer before being handed to the SSH channel,
 * cutting the number of tiny network writes. A flush is triggered when the
 * buffer fills, an idle gap elapses, or a "must-send" byte (Enter, Ctrl-C,
 * ESC) is seen.
 *
 * Self-contained C99 library module. No project headers, no windows.h.
 * All functions are prefixed inputcoal_ to avoid linker collisions.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define INPUTCOAL_CAP      1024
#define INPUTCOAL_IDLE_MS  8        /* default flush-after-quiet window */

typedef struct inputcoal_buf {
    unsigned char buf[INPUTCOAL_CAP];
    int       len;            /* bytes currently buffered          */
    uint64_t  last_ms;        /* timestamp of last push            */
    int       idle_ms;        /* idle threshold before flush       */
    int       force_pending;  /* a must-send byte was queued       */
    uint64_t  bytes_in;       /* lifetime bytes accepted           */
    uint64_t  bytes_out;      /* lifetime bytes flushed            */
    uint64_t  flushes;        /* lifetime flush count              */
} inputcoal_buf;

/* A byte that must not linger in the buffer: Enter, Ctrl-C, Ctrl-D, ESC. */
static int inputcoal_is_force(unsigned char b)
{
    return (b == '\r' || b == '\n' || b == 0x03 || b == 0x04 || b == 0x1b);
}

int inputcoal_init(inputcoal_buf *c)
{
    if (!c)
        return -1;
    memset(c, 0, sizeof(*c));
    c->idle_ms = INPUTCOAL_IDLE_MS;
    return 0;
}

int inputcoal_set_idle(inputcoal_buf *c, int idle_ms)
{
    if (!c || idle_ms <= 0)
        return -1;
    c->idle_ms = idle_ms;
    return 0;
}

/* Append up to n bytes at time now_ms. Returns bytes accepted, -1 on error. */
int inputcoal_push(inputcoal_buf *c, const unsigned char *data, int n,
                   uint64_t now_ms)
{
    int i;
    if (!c || (!data && n > 0) || n < 0)
        return -1;
    for (i = 0; i < n; i++) {
        if (c->len >= INPUTCOAL_CAP)
            break;
        c->buf[c->len++] = data[i];
        c->bytes_in++;
        if (inputcoal_is_force(data[i]))
            c->force_pending = 1;
    }
    c->last_ms = now_ms;
    return i;
}

/* Returns 1 if a flush is due (full, forced, or idle gap exceeded), else 0. */
int inputcoal_should_flush(const inputcoal_buf *c, uint64_t now_ms)
{
    if (!c || c->len == 0)
        return 0;
    if (c->force_pending)
        return 1;
    if (c->len >= INPUTCOAL_CAP)
        return 1;
    if (now_ms >= c->last_ms &&
        (now_ms - c->last_ms) >= (uint64_t)c->idle_ms)
        return 1;
    return 0;
}

/* Drain coalesced bytes into out (capacity out_cap); returns count or -1. */
int inputcoal_flush(inputcoal_buf *c, unsigned char *out, int out_cap)
{
    int n;
    if (!c || !out || out_cap <= 0)
        return -1;
    n = (c->len < out_cap) ? c->len : out_cap;
    if (n > 0)
        memcpy(out, c->buf, (size_t)n);
    if (n < c->len) {
        /* Partial drain: keep the remainder for the next flush. */
        memmove(c->buf, c->buf + n, (size_t)(c->len - n));
        c->len -= n;
    } else {
        c->len = 0;
        c->force_pending = 0;
    }
    c->bytes_out += (uint64_t)n;
    c->flushes++;
    return n;
}

int inputcoal_pending(const inputcoal_buf *c)
{
    return c ? c->len : 0;
}

/* Render a stats line into buf; returns chars written (excl NUL) or -1. */
int inputcoal_stats(const inputcoal_buf *c, char *buf, int bufsize)
{
    int w;
    if (!c || !buf || bufsize <= 0)
        return -1;
    w = snprintf(buf, (size_t)bufsize,
                 "inputcoal: %d/%d queued in=%llu out=%llu flushes=%llu force=%d",
                 c->len, INPUTCOAL_CAP,
                 (unsigned long long)c->bytes_in,
                 (unsigned long long)c->bytes_out,
                 (unsigned long long)c->flushes, c->force_pending);
    if (w < 0 || w >= bufsize)
        return -1;
    return w;
}
