/*
 * puttyalt_writebatch.c - Output write batching for PuttyAlt.
 *
 * Accumulates terminal/network output into a ring of segments and emits them
 * as a small number of large writes rather than many small ones. Tracks a
 * watermark so the caller can apply back-pressure, and reports how effectively
 * writes were coalesced.
 *
 * Self-contained C99 library module. No project headers, no windows.h.
 * All functions are prefixed writebatch_ to avoid linker collisions.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define WRITEBATCH_CAP        16384
#define WRITEBATCH_HIGH_WATER 12288   /* apply back-pressure above this */

typedef struct WriteBatch {
    unsigned char buf[WRITEBATCH_CAP];
    int       len;
    int       appends;       /* appends since last drain        */
    uint64_t  total_bytes;   /* lifetime bytes appended         */
    uint64_t  total_appends; /* lifetime append calls           */
    uint64_t  drains;        /* lifetime drain calls            */
    int       overflow;      /* count of rejected appends       */
} WriteBatch;

void writebatch_init(WriteBatch *w)
{
    if (!w)
        return;
    memset(w, 0, sizeof(*w));
}

/* Append n bytes. Returns the number accepted (0..n); a short return means
 * the batch is full and the caller should drain before retrying. */
int writebatch_append(WriteBatch *w, const unsigned char *data, int n)
{
    int room, take;
    if (!w || !data || n <= 0)
        return 0;
    room = WRITEBATCH_CAP - w->len;
    if (room <= 0) {
        w->overflow++;
        return 0;
    }
    take = n < room ? n : room;
    memcpy(w->buf + w->len, data, (size_t)take);
    w->len += take;
    w->appends++;
    w->total_bytes += (uint64_t)take;
    w->total_appends++;
    if (take < n)
        w->overflow++;
    return take;
}

/* Returns 1 if the batch is at/over the high-water mark and the caller
 * should stop reading more input until it drains. */
int writebatch_pressure(const WriteBatch *w)
{
    return (w && w->len >= WRITEBATCH_HIGH_WATER) ? 1 : 0;
}

int writebatch_ready(const WriteBatch *w)
{
    return (w && w->len > 0) ? 1 : 0;
}

/* Expose the contiguous batch for a single write() call without copying.
 * Sets *out to the internal buffer and returns its length. */
int writebatch_peek(const WriteBatch *w, const unsigned char **out)
{
    if (!w || !out)
        return 0;
    *out = w->buf;
    return w->len;
}

/* Mark `n` leading bytes as written. Handles partial writes by shifting the
 * remainder down. Returns bytes still pending, or -1 on error. */
int writebatch_consume(WriteBatch *w, int n)
{
    if (!w || n < 0)
        return -1;
    if (n >= w->len) {
        w->len = 0;
        w->appends = 0;
    } else {
        memmove(w->buf, w->buf + n, (size_t)(w->len - n));
        w->len -= n;
    }
    w->drains++;
    return w->len;
}

int writebatch_pending(const WriteBatch *w)
{
    return w ? w->len : 0;
}

/* Report batching effectiveness into buf. Returns bytes written (excl NUL). */
int writebatch_stats(const WriteBatch *w, char *buf, int bufsize)
{
    double ratio;
    if (!w || !buf || bufsize <= 0)
        return 0;
    ratio = w->drains ? (double)w->total_appends / (double)w->drains : 0.0;
    return snprintf(buf, (size_t)bufsize,
                    "writebatch: %d/%d buffered, coalesce=%.1f, ovf=%d",
                    w->len, WRITEBATCH_CAP, ratio, w->overflow);
}
