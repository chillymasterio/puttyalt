/*
 * puttyalt_cellslab.c - Slab pool allocator for terminal cells in PuttyAlt.
 *
 * Allocates fixed-size terminal cell records from pre-sliced slabs with an
 * intrusive free list, giving O(1) acquire/release and stable indices that
 * can be stored compactly in line buffers.
 *
 * Self-contained C99 library module. No project headers, no windows.h.
 * All functions are prefixed cellslab_ to avoid linker collisions.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define CELLSLAB_CAPACITY 4096   /* total cells in the pool */
#define CELLSLAB_NIL      (-1)

typedef struct CellSlabCell {
    uint32_t codepoint;
    uint32_t fg;
    uint32_t bg;
    uint16_t attr;
    int32_t  next;   /* free-list link when unused */
} CellSlabCell;

typedef struct CellSlab {
    CellSlabCell cells[CELLSLAB_CAPACITY];
    int32_t      free_head;   /* head of free list, or NIL    */
    int          in_use;      /* currently allocated cells    */
    int          high_water;  /* peak simultaneous allocation */
    int          inited;      /* 1 once the free list is built*/
} CellSlab;

void cellslab_init(CellSlab *s)
{
    int i;
    if (!s)
        return;
    memset(s, 0, sizeof(*s));
    for (i = 0; i < CELLSLAB_CAPACITY - 1; i++)
        s->cells[i].next = i + 1;
    s->cells[CELLSLAB_CAPACITY - 1].next = CELLSLAB_NIL;
    s->free_head = 0;
    s->in_use = 0;
    s->high_water = 0;
    s->inited = 1;
}

/* Acquire a cell. Returns its index [0,CAPACITY) or -1 if the slab is full. */
int cellslab_acquire(CellSlab *s)
{
    int idx;
    CellSlabCell *c;
    if (!s || !s->inited || s->free_head == CELLSLAB_NIL)
        return -1;
    idx = s->free_head;
    c = &s->cells[idx];
    s->free_head = c->next;
    c->next = CELLSLAB_NIL;
    c->codepoint = 0;
    c->fg = 0;
    c->bg = 0;
    c->attr = 0;
    s->in_use++;
    if (s->in_use > s->high_water)
        s->high_water = s->in_use;
    return idx;
}

/* Release a previously-acquired cell back to the free list.
 * Returns 0 on success, -1 on bad index. */
int cellslab_release(CellSlab *s, int idx)
{
    CellSlabCell *c;
    if (!s || !s->inited || idx < 0 || idx >= CELLSLAB_CAPACITY)
        return -1;
    c = &s->cells[idx];
    if (c->next != CELLSLAB_NIL)   /* already on free list */
        return -1;
    /* Detect double-free heuristically: a free cell's next would not be NIL,
     * but freshly-released cells must link into the list head. */
    c->next = s->free_head;
    s->free_head = idx;
    if (s->in_use > 0)
        s->in_use--;
    return 0;
}

/* Store glyph data into an acquired cell. Returns 0 on success. */
int cellslab_set(CellSlab *s, int idx, uint32_t cp, uint32_t fg,
                 uint32_t bg, uint16_t attr)
{
    CellSlabCell *c;
    if (!s || !s->inited || idx < 0 || idx >= CELLSLAB_CAPACITY)
        return -1;
    c = &s->cells[idx];
    if (c->next != CELLSLAB_NIL)   /* not currently allocated */
        return -1;
    c->codepoint = cp;
    c->fg = fg;
    c->bg = bg;
    c->attr = attr;
    return 0;
}

/* Read glyph data from an allocated cell. Out-pointers may be NULL.
 * Returns 0 on success, -1 on bad index or freed cell. */
int cellslab_get(const CellSlab *s, int idx, uint32_t *cp, uint32_t *fg,
                 uint32_t *bg, uint16_t *attr)
{
    const CellSlabCell *c;
    if (!s || !s->inited || idx < 0 || idx >= CELLSLAB_CAPACITY)
        return -1;
    c = &s->cells[idx];
    if (c->next != CELLSLAB_NIL)
        return -1;
    if (cp)   *cp = c->codepoint;
    if (fg)   *fg = c->fg;
    if (bg)   *bg = c->bg;
    if (attr) *attr = c->attr;
    return 0;
}

int cellslab_available(const CellSlab *s)
{
    if (!s || !s->inited)
        return 0;
    return CELLSLAB_CAPACITY - s->in_use;
}

/* Report pool usage into buf. Returns bytes written (excluding NUL). */
int cellslab_stats(const CellSlab *s, char *buf, int bufsize)
{
    if (!s || !buf || bufsize <= 0)
        return 0;
    return snprintf(buf, (size_t)bufsize,
                    "cellslab: %d/%d in use, peak=%d, free=%d",
                    s->in_use, CELLSLAB_CAPACITY, s->high_water,
                    CELLSLAB_CAPACITY - s->in_use);
}
