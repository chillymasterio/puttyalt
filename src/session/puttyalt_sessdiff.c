#include "puttyalt_sessdiff.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void sessdiff_init(DiffResult *dr)
{
    memset(dr, 0, sizeof(*dr));
    dr->capacity = 256;
    dr->lines = (DiffLine *)calloc(dr->capacity, sizeof(DiffLine));
}

void sessdiff_free(DiffResult *dr) { free(dr->lines); memset(dr, 0, sizeof(*dr)); }

static void add_diff(DiffResult *dr, DiffType type, int la, int lb, const char *text)
{
    if (dr->count >= dr->capacity) {
        dr->capacity *= 2;
        dr->lines = (DiffLine *)realloc(dr->lines, dr->capacity * sizeof(DiffLine));
    }
    DiffLine *d = &dr->lines[dr->count++];
    d->type = type;
    d->line_a = la;
    d->line_b = lb;
    snprintf(d->text, sizeof(d->text), "%s", text ? text : "");
    switch (type) {
        case DIFF_ADD: dr->adds++; break;
        case DIFF_DEL: dr->dels++; break;
        case DIFF_EQUAL: dr->equals++; break;
    }
}

int sessdiff_compare(DiffResult *dr, const char **a, int ac, const char **b, int bc)
{
    /* simple LCS-based diff */
    int ai = 0, bi = 0;
    while (ai < ac && bi < bc) {
        if (strcmp(a[ai], b[bi]) == 0) {
            add_diff(dr, DIFF_EQUAL, ai, bi, a[ai]);
            ai++; bi++;
        } else {
            /* look ahead to find next match */
            int found_a = -1, found_b = -1;
            for (int j = bi + 1; j < bc && j < bi + 5; j++) {
                if (strcmp(a[ai], b[j]) == 0) { found_b = j; break; }
            }
            for (int j = ai + 1; j < ac && j < ai + 5; j++) {
                if (strcmp(a[j], b[bi]) == 0) { found_a = j; break; }
            }
            if (found_b >= 0 && (found_a < 0 || (found_b - bi) <= (found_a - ai))) {
                while (bi < found_b) { add_diff(dr, DIFF_ADD, -1, bi, b[bi]); bi++; }
            } else if (found_a >= 0) {
                while (ai < found_a) { add_diff(dr, DIFF_DEL, ai, -1, a[ai]); ai++; }
            } else {
                add_diff(dr, DIFF_DEL, ai, -1, a[ai]); ai++;
                add_diff(dr, DIFF_ADD, -1, bi, b[bi]); bi++;
            }
        }
    }
    while (ai < ac) { add_diff(dr, DIFF_DEL, ai, -1, a[ai]); ai++; }
    while (bi < bc) { add_diff(dr, DIFF_ADD, -1, bi, b[bi]); bi++; }
    return dr->count;
}

DiffLine *sessdiff_get(DiffResult *dr, int index)
{
    return (index >= 0 && index < dr->count) ? &dr->lines[index] : NULL;
}

void sessdiff_stats(DiffResult *dr, int *adds, int *dels, int *equals)
{
    if (adds) *adds = dr->adds;
    if (dels) *dels = dr->dels;
    if (equals) *equals = dr->equals;
}
