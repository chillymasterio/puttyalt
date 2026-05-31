/* puttyalt_blockrect.c - Rectangular (column-block) terminal selection model. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCKRECT_MAX_DIM 4096

typedef struct { int x; int y; } blockrect_point;

typedef struct {
    int active;             /* 1 once an anchor has been set, else 0 */
    blockrect_point anchor; /* fixed corner where selection began */
    blockrect_point cursor; /* moving corner under the pointer */
} blockrect;

static int blockrect_clamp(int v)
{
    if (v < 0) return 0;
    if (v >= BLOCKRECT_MAX_DIM) return BLOCKRECT_MAX_DIM - 1;
    return v;
}

void blockrect_clear(blockrect *b) { if (b) memset(b, 0, sizeof(*b)); }

int blockrect_begin(blockrect *b, int x, int y)
{
    if (!b) return -1;
    b->anchor.x = b->cursor.x = blockrect_clamp(x);
    b->anchor.y = b->cursor.y = blockrect_clamp(y);
    b->active = 1;
    return 0;
}

int blockrect_extend(blockrect *b, int x, int y)
{
    if (!b || !b->active) return -1;
    b->cursor.x = blockrect_clamp(x);
    b->cursor.y = blockrect_clamp(y);
    return 0;
}

int blockrect_active(const blockrect *b) { return (b && b->active) ? 1 : 0; }

int blockrect_top(const blockrect *b)
{ if (!b || !b->active) return -1; return (b->anchor.y < b->cursor.y) ? b->anchor.y : b->cursor.y; }

int blockrect_bottom(const blockrect *b)
{ if (!b || !b->active) return -1; return (b->anchor.y > b->cursor.y) ? b->anchor.y : b->cursor.y; }

int blockrect_left(const blockrect *b)
{ if (!b || !b->active) return -1; return (b->anchor.x < b->cursor.x) ? b->anchor.x : b->cursor.x; }

int blockrect_right(const blockrect *b)
{ if (!b || !b->active) return -1; return (b->anchor.x > b->cursor.x) ? b->anchor.x : b->cursor.x; }

int blockrect_contains(const blockrect *b, int x, int y)
{
    if (!b || !b->active) return 0;
    if (x < blockrect_left(b) || x > blockrect_right(b)) return 0;
    if (y < blockrect_top(b) || y > blockrect_bottom(b)) return 0;
    return 1;
}

int blockrect_rows(const blockrect *b)
{
    if (!b || !b->active) return -1;
    return blockrect_bottom(b) - blockrect_top(b) + 1;
}

/* Every row in a block shares the same [left,right] span; report it. */
int blockrect_row_span(const blockrect *b, int row, int *left, int *right)
{
    if (!b || !b->active) return -1;
    if (row < blockrect_top(b) || row > blockrect_bottom(b)) return -1;
    if (left) *left = blockrect_left(b);
    if (right) *right = blockrect_right(b);
    return 0;
}

int blockrect_cell_count(const blockrect *b)
{
    if (!b || !b->active) return -1;
    return blockrect_rows(b) * (blockrect_right(b) - blockrect_left(b) + 1);
}

int blockrect_describe(const blockrect *b, char *out, size_t cap)
{
    int n;
    if (!out || cap == 0) return -1;
    if (!b || !b->active)
        n = snprintf(out, cap, "block: inactive");
    else
        n = snprintf(out, cap,
                     "block: cols %d-%d rows %d-%d (%d x %d, %d cells)",
                     blockrect_left(b), blockrect_right(b),
                     blockrect_top(b), blockrect_bottom(b),
                     blockrect_right(b) - blockrect_left(b) + 1,
                     blockrect_rows(b), blockrect_cell_count(b));
    if (n < 0 || (size_t)n >= cap) return -1;
    return n;
}
