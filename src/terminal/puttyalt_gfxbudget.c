/*
 * puttyalt_gfxbudget.c - graphics memory budget ledger for stored images.
 *
 * Maintains a byte-budget ledger: charges/credits per-image allocations and
 * selects the least-recently-used image for eviction when over budget.
 */

#include <string.h>
#include <stdio.h>

#define GFXBUDGET_MAX_IMAGES 256

typedef struct {
    int  id;          /* image id, -1 when slot is free            */
    long bytes;       /* bytes charged for this image              */
    long touched_at;  /* monotonically increasing recency stamp    */
} GfxImageEntry;

typedef struct {
    GfxImageEntry entries[GFXBUDGET_MAX_IMAGES];
    int           count;       /* number of occupied slots          */
    long          max_bytes;   /* configured budget ceiling         */
    long          used_bytes;  /* sum of charged bytes              */
    long          tick;        /* recency counter, bumped per use   */
} GfxBudget;

void gfxbudget_init(GfxBudget *b, long max_bytes)
{
    int i;
    if (b == NULL)
        return;
    memset(b, 0, sizeof(*b));
    b->max_bytes = (max_bytes < 0) ? 0 : max_bytes;
    b->count = 0;
    b->used_bytes = 0;
    b->tick = 0;
    for (i = 0; i < GFXBUDGET_MAX_IMAGES; i++)
        b->entries[i].id = -1;
}

static int gfxbudget_find(const GfxBudget *b, int img_id)
{
    int i;
    for (i = 0; i < GFXBUDGET_MAX_IMAGES; i++)
        if (b->entries[i].id == img_id)
            return i;
    return -1;
}

int gfxbudget_charge(GfxBudget *b, int img_id, long bytes)
{
    int i, slot = -1;
    if (b == NULL || img_id < 0 || bytes < 0)
        return -1;

    i = gfxbudget_find(b, img_id);
    if (i >= 0) {
        /* Re-charge an existing image: adjust the running total. */
        b->used_bytes += bytes - b->entries[i].bytes;
        b->entries[i].bytes = bytes;
        b->entries[i].touched_at = ++b->tick;
        return 0;
    }

    for (i = 0; i < GFXBUDGET_MAX_IMAGES; i++) {
        if (b->entries[i].id == -1) {
            slot = i;
            break;
        }
    }
    if (slot < 0)
        return -1; /* table full */

    b->entries[slot].id = img_id;
    b->entries[slot].bytes = bytes;
    b->entries[slot].touched_at = ++b->tick;
    b->used_bytes += bytes;
    b->count++;
    return 0;
}

void gfxbudget_credit(GfxBudget *b, int img_id)
{
    int i;
    if (b == NULL || img_id < 0)
        return;
    i = gfxbudget_find(b, img_id);
    if (i < 0)
        return;
    b->used_bytes -= b->entries[i].bytes;
    if (b->used_bytes < 0)
        b->used_bytes = 0;
    b->entries[i].id = -1;
    b->entries[i].bytes = 0;
    b->entries[i].touched_at = 0;
    if (b->count > 0)
        b->count--;
}

void gfxbudget_touch(GfxBudget *b, int img_id)
{
    int i;
    if (b == NULL || img_id < 0)
        return;
    i = gfxbudget_find(b, img_id);
    if (i >= 0)
        b->entries[i].touched_at = ++b->tick;
}

int gfxbudget_evict_lru(GfxBudget *b, int *evicted_id)
{
    int i, lru = -1;
    long oldest = 0;
    if (b == NULL)
        return -1;
    if (b->used_bytes <= b->max_bytes)
        return 0; /* nothing to evict */

    for (i = 0; i < GFXBUDGET_MAX_IMAGES; i++) {
        if (b->entries[i].id == -1)
            continue;
        if (lru < 0 || b->entries[i].touched_at < oldest) {
            oldest = b->entries[i].touched_at;
            lru = i;
        }
    }
    if (lru < 0)
        return 0; /* over budget but no images recorded */

    if (evicted_id != NULL)
        *evicted_id = b->entries[lru].id;
    gfxbudget_credit(b, b->entries[lru].id);
    return 1; /* one image evicted */
}

long gfxbudget_used(const GfxBudget *b)
{
    if (b == NULL)
        return 0;
    return b->used_bytes;
}
