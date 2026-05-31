/*
 * puttyalt_readsizer.c - Adaptive read-buffer sizing for PuttyAlt.
 *
 * Chooses the size of the next socket read based on recent fill behaviour: if
 * reads keep coming back full, the buffer grows (geometrically) to drain bulk
 * transfers in fewer syscalls; when reads keep coming back short, it shrinks so
 * idle interactive sessions keep their memory footprint small. The size is
 * always clamped to configured min/max bounds.
 *
 * Self-contained C99 library module. Standard headers only: no project headers,
 * no windows.h. All symbols are prefixed readsizer_ to avoid collisions. Where
 * a timestamp could be useful it is passed in as now_ms (no time-of-day calls).
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define READSIZER_HISTORY 16   /* fixed-size ring of recent read sizes */

typedef struct readsizer {
    uint32_t min_size;      /* lower clamp bound (bytes)              */
    uint32_t max_size;      /* upper clamp bound (bytes)             */
    uint32_t cur_size;      /* current recommended read size         */
    uint32_t full_streak;   /* consecutive near-full reads           */
    uint32_t short_streak;  /* consecutive short reads               */
    uint32_t grow_after;    /* full reads needed before growing      */
    uint32_t shrink_after;  /* short reads needed before shrinking   */
    uint32_t hist[READSIZER_HISTORY]; /* recent observed read sizes  */
    uint32_t hist_count;    /* valid entries in hist                 */
    uint32_t hist_pos;      /* next write index into hist (ring)     */
    uint64_t total_reads;   /* lifetime reads observed               */
    uint64_t total_bytes;   /* lifetime bytes read                   */
    uint64_t last_ms;       /* now_ms of most recent observation     */
} readsizer;

int readsizer_init(readsizer *r, uint32_t min_size, uint32_t max_size)
{
    if (!r || min_size == 0 || max_size < min_size)
        return -1;
    memset(r, 0, sizeof(*r));
    r->min_size = min_size;
    r->max_size = max_size;
    r->cur_size = min_size;
    r->grow_after = 3;
    r->shrink_after = 4;
    return 0;
}

/* Buffer size to offer for the next read. */
uint32_t readsizer_next(const readsizer *r)
{
    return r ? r->cur_size : 0;
}

/* Force the recommended size back to the minimum (e.g. after a stall or
 * reconnect) without clearing lifetime counters. */
int readsizer_reset_size(readsizer *r)
{
    if (!r)
        return -1;
    r->cur_size = r->min_size;
    r->full_streak = 0;
    r->short_streak = 0;
    return 0;
}

/* Feed back the result of a read: bytes_read were returned when cur_size bytes
 * of buffer were offered. A read is "full" when it returns at least 90% of the
 * offered size. Grows/shrinks on sustained streaks, clamped to bounds. */
int readsizer_observe(readsizer *r, uint32_t bytes_read, uint64_t now_ms)
{
    if (!r)
        return -1;

    r->hist[r->hist_pos] = bytes_read;
    r->hist_pos = (r->hist_pos + 1u) % READSIZER_HISTORY;
    if (r->hist_count < READSIZER_HISTORY)
        r->hist_count++;
    r->total_reads++;
    r->total_bytes += bytes_read;
    r->last_ms = now_ms;

    if (bytes_read >= r->cur_size - (r->cur_size / 10u)) {   /* >= 90% */
        r->full_streak++;
        r->short_streak = 0;
        if (r->full_streak >= r->grow_after && r->cur_size < r->max_size) {
            uint64_t grown = (uint64_t)r->cur_size * 2u;
            r->cur_size = grown > r->max_size ? r->max_size : (uint32_t)grown;
            r->full_streak = 0;
        }
    } else {
        r->short_streak++;
        r->full_streak = 0;
        if (r->short_streak >= r->shrink_after && r->cur_size > r->min_size) {
            uint32_t shrunk = r->cur_size / 2u;
            r->cur_size = shrunk < r->min_size ? r->min_size : shrunk;
            r->short_streak = 0;
        }
    }
    return 0;
}

/* Average bytes per read over the recent history window, 0 if none. */
uint32_t readsizer_avg_read(const readsizer *r)
{
    uint64_t sum = 0;
    uint32_t i;
    if (!r || r->hist_count == 0)
        return 0;
    for (i = 0; i < r->hist_count; i++)
        sum += r->hist[i];
    return (uint32_t)(sum / r->hist_count);
}

/* Report sizing state into buf. Returns bytes written (excluding NUL), or -1
 * on bad args or truncation. */
int readsizer_stats(const readsizer *r, char *buf, size_t buflen)
{
    int n;
    if (!r || !buf || buflen == 0)
        return -1;
    n = snprintf(buf, buflen,
                 "readsizer cur=%u min=%u max=%u avg=%u reads=%llu bytes=%llu "
                 "full=%u short=%u",
                 r->cur_size, r->min_size, r->max_size, readsizer_avg_read(r),
                 (unsigned long long)r->total_reads,
                 (unsigned long long)r->total_bytes,
                 r->full_streak, r->short_streak);
    if (n < 0 || (size_t)n >= buflen)
        return -1;
    return n;
}
