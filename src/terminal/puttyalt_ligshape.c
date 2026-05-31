/* puttyalt_ligshape.c - HarfBuzz-style ligature shaping table for PuttyAlt. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LIGSHAPE_MAX_RULES 64
#define LIGSHAPE_MAX_SEQ   8
#define LIGSHAPE_MAX_RUN   256

typedef struct {
    uint32_t seq[LIGSHAPE_MAX_SEQ]; /* input codepoint sequence */
    int      len;                   /* number of codepoints in seq */
    uint32_t glyph;                 /* shaped glyph id produced */
} ligshape_rule;

typedef struct {
    ligshape_rule rules[LIGSHAPE_MAX_RULES];
    int           count;
} ligshape_table;

void ligshape_init(ligshape_table *t)
{
    if (t) { memset(t, 0, sizeof(*t)); t->count = 0; }
}

void ligshape_clear(ligshape_table *t)
{
    if (t) t->count = 0;
}

int ligshape_rule_count(const ligshape_table *t)
{
    return t ? t->count : -1;
}

int ligshape_add_rule(ligshape_table *t, const uint32_t *seq, int len,
                      uint32_t glyph)
{
    ligshape_rule *r;
    int i;

    if (!t || !seq || len < 1 || len > LIGSHAPE_MAX_SEQ)
        return -1;
    if (t->count >= LIGSHAPE_MAX_RULES)
        return -1;
    r = &t->rules[t->count];
    for (i = 0; i < len; i++)
        r->seq[i] = seq[i];
    r->len = len;
    r->glyph = glyph;
    t->count++;
    return 0;
}

/* Return matched length (>0) of the longest rule at run[pos], glyph via out. */
int ligshape_match_at(const ligshape_table *t, const uint32_t *run, int runlen,
                      int pos, uint32_t *out_glyph)
{
    int i, j, best = 0;
    uint32_t best_glyph = 0;

    if (!t || !run || pos < 0 || pos >= runlen)
        return -1;
    for (i = 0; i < t->count; i++) {
        const ligshape_rule *r = &t->rules[i];
        if (r->len <= best || pos + r->len > runlen)
            continue;
        for (j = 0; j < r->len; j++) {
            if (run[pos + j] != r->seq[j])
                break;
        }
        if (j == r->len) {
            best = r->len;
            best_glyph = r->glyph;
        }
    }
    if (best > 0 && out_glyph)
        *out_glyph = best_glyph;
    return best;
}

/* Shape run into out (cap entries); returns shaped length or -1 on error. */
int ligshape_shape_run(const ligshape_table *t, const uint32_t *run, int runlen,
                       uint32_t *out, int cap)
{
    int pos = 0, n = 0;

    if (!t || !run || !out || runlen < 0 || runlen > LIGSHAPE_MAX_RUN || cap < 0)
        return -1;
    while (pos < runlen) {
        uint32_t glyph = 0;
        int m = ligshape_match_at(t, run, runlen, pos, &glyph);
        if (n >= cap)
            return -1;
        if (m > 0) {
            out[n++] = glyph;
            pos += m;
        } else {
            out[n++] = run[pos++];
        }
    }
    return n;
}

/* Format a human-readable dump of rule index into buf. Returns chars or -1. */
int ligshape_describe(const ligshape_table *t, int index, char *buf, size_t cap)
{
    const ligshape_rule *r;
    size_t off = 0;
    int i, w;

    if (!t || !buf || cap == 0 || index < 0 || index >= t->count)
        return -1;
    r = &t->rules[index];
    w = snprintf(buf, cap, "rule[%d]: ", index);
    if (w < 0 || (size_t)w >= cap)
        return -1;
    off = (size_t)w;
    for (i = 0; i < r->len && off < cap; i++) {
        w = snprintf(buf + off, cap - off, "%s%04X",
                     i ? " " : "", (unsigned)r->seq[i]);
        if (w < 0 || (size_t)w >= cap - off)
            return -1;
        off += (size_t)w;
    }
    w = snprintf(buf + off, cap - off, " -> %04X", (unsigned)r->glyph);
    if (w < 0 || (size_t)w >= cap - off)
        return -1;
    off += (size_t)w;
    return (int)off;
}
