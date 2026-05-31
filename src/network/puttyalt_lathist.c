/*
 * puttyalt_lathist.c - Latency histogram for PuttyAlt.
 *
 * Records round-trip latency samples into logarithmic-ish fixed buckets and
 * derives approximate percentiles (p50/p90/p99) without storing the raw
 * sample stream. Suitable for a long-running connection health display.
 *
 * Self-contained C99 library module. No project headers, no windows.h.
 * All functions are prefixed lathist_ to avoid linker collisions.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define LATHIST_BUCKETS 12

/* Upper bound (inclusive, milliseconds) of each bucket; last is a catch-all. */
static const int lathist_bounds[LATHIST_BUCKETS] = {
    1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 0x7fffffff
};

typedef struct LatHist {
    uint64_t counts[LATHIST_BUCKETS];
    uint64_t total;       /* total samples recorded */
    double   sum_ms;      /* running sum for mean    */
    int      min_ms;      /* observed minimum        */
    int      max_ms;      /* observed maximum        */
} LatHist;

void lathist_init(LatHist *h)
{
    if (!h)
        return;
    memset(h, 0, sizeof(*h));
    h->min_ms = -1;
    h->max_ms = -1;
}

static int lathist_bucket_of(int ms)
{
    int i;
    if (ms < 0)
        ms = 0;
    for (i = 0; i < LATHIST_BUCKETS; i++) {
        if (ms <= lathist_bounds[i])
            return i;
    }
    return LATHIST_BUCKETS - 1;
}

/* Record a latency sample in milliseconds. Returns the bucket index used. */
int lathist_record(LatHist *h, int ms)
{
    int b;
    if (!h)
        return -1;
    if (ms < 0)
        ms = 0;
    b = lathist_bucket_of(ms);
    h->counts[b]++;
    h->total++;
    h->sum_ms += (double)ms;
    if (h->min_ms < 0 || ms < h->min_ms)
        h->min_ms = ms;
    if (h->max_ms < 0 || ms > h->max_ms)
        h->max_ms = ms;
    return b;
}

/* Approximate percentile (0..100). Returns the upper bound (ms) of the bucket
 * containing the requested rank, or -1 if no samples exist. The catch-all
 * bucket reports the observed maximum instead of INT_MAX. */
int lathist_percentile(const LatHist *h, int pct)
{
    uint64_t target, acc = 0;
    int i;
    if (!h || h->total == 0)
        return -1;
    if (pct < 0)   pct = 0;
    if (pct > 100) pct = 100;
    target = (h->total * (uint64_t)pct + 99) / 100;   /* ceil */
    if (target == 0)
        target = 1;
    for (i = 0; i < LATHIST_BUCKETS; i++) {
        acc += h->counts[i];
        if (acc >= target) {
            if (i == LATHIST_BUCKETS - 1)
                return h->max_ms;
            return lathist_bounds[i];
        }
    }
    return h->max_ms;
}

/* Mean latency in milliseconds, or -1.0 if no samples. */
double lathist_mean(const LatHist *h)
{
    if (!h || h->total == 0)
        return -1.0;
    return h->sum_ms / (double)h->total;
}

void lathist_reset(LatHist *h)
{
    lathist_init(h);
}

/* Render an ASCII bar chart of the histogram into buf, one line per non-empty
 * bucket. Returns bytes written (excluding NUL). */
int lathist_render(const LatHist *h, char *buf, int bufsize)
{
    int i, off = 0;
    uint64_t peak = 0;
    if (!h || !buf || bufsize <= 0)
        return 0;
    for (i = 0; i < LATHIST_BUCKETS; i++)
        if (h->counts[i] > peak)
            peak = h->counts[i];
    if (peak == 0) {
        return snprintf(buf, (size_t)bufsize, "lathist: (no samples)");
    }
    for (i = 0; i < LATHIST_BUCKETS && off < bufsize - 1; i++) {
        int bars, j;
        char label[16];
        if (h->counts[i] == 0)
            continue;
        if (i == LATHIST_BUCKETS - 1)
            snprintf(label, sizeof(label), ">%dms", lathist_bounds[i - 1]);
        else
            snprintf(label, sizeof(label), "<=%dms", lathist_bounds[i]);
        bars = (int)((h->counts[i] * 20) / peak);
        off += snprintf(buf + off, (size_t)(bufsize - off), "%-8s ", label);
        for (j = 0; j < bars && off < bufsize - 1; j++)
            buf[off++] = '#';
        off += snprintf(buf + off, (size_t)(bufsize - off),
                        " %llu\n", (unsigned long long)h->counts[i]);
    }
    if (off < bufsize)
        buf[off] = '\0';
    return off;
}

/* Report a one-line summary into buf. Returns bytes written (excl NUL). */
int lathist_summary(const LatHist *h, char *buf, int bufsize)
{
    if (!h || !buf || bufsize <= 0)
        return 0;
    if (h->total == 0)
        return snprintf(buf, (size_t)bufsize, "lathist: 0 samples");
    return snprintf(buf, (size_t)bufsize,
                    "lathist: n=%llu mean=%.1f p50=%d p90=%d p99=%d max=%d",
                    (unsigned long long)h->total, lathist_mean(h),
                    lathist_percentile(h, 50), lathist_percentile(h, 90),
                    lathist_percentile(h, 99), h->max_ms);
}
