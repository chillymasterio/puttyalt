/* puttyalt_graphwidth.c - Grapheme cluster width table for PuttyAlt. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct { uint32_t lo, hi; } gw_range;

/* Combining marks, default-ignorables, variation selectors -> 0 columns. */
static const gw_range gw_zero[] = {
    {0x0300, 0x036F}, {0x0483, 0x0489}, {0x0591, 0x05BD}, {0x064B, 0x065F},
    {0x0670, 0x0670}, {0x0E34, 0x0E3A}, {0x200B, 0x200F}, {0x202A, 0x202E},
    {0x2060, 0x2064}, {0xFE00, 0xFE0F}, {0xFE20, 0xFE2F}, {0xE0100, 0xE01EF},
};

/* East-Asian Wide/Fullwidth and core emoji -> 2 columns. */
static const gw_range gw_wide[] = {
    {0x1100, 0x115F}, {0x2329, 0x232A}, {0x2E80, 0x303E}, {0x3041, 0x33FF},
    {0x3400, 0x4DBF}, {0x4E00, 0x9FFF}, {0xA000, 0xA4CF}, {0xAC00, 0xD7A3},
    {0xF900, 0xFAFF}, {0xFE30, 0xFE4F}, {0xFF00, 0xFF60}, {0xFFE0, 0xFFE6},
    {0x1F300, 0x1F64F}, {0x1F900, 0x1FAFF}, {0x20000, 0x3FFFD},
};

#define GW_ZWJ 0x200Du

typedef struct {
    int columns;   /* total display columns of the cluster */
    size_t bytes;  /* UTF-8 bytes consumed */
} gw_cluster;

static int gw_in(const gw_range *t, size_t n, uint32_t cp) {
    size_t i;
    for (i = 0; i < n; i++)
        if (cp >= t[i].lo && cp <= t[i].hi) return 1;
    return 0;
}

int graphwidth_is_zero(uint32_t cp) {
    if (cp == 0) return 1;
    return gw_in(gw_zero, sizeof gw_zero / sizeof gw_zero[0], cp);
}

int graphwidth_is_wide(uint32_t cp) {
    return gw_in(gw_wide, sizeof gw_wide / sizeof gw_wide[0], cp);
}

int graphwidth_cp(uint32_t cp) {
    if (cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) return -1;
    if (cp < 0x20 || (cp >= 0x7F && cp < 0xA0)) return 0;
    if (graphwidth_is_zero(cp)) return 0;
    if (graphwidth_is_wide(cp)) return 2;
    return 1;
}

/* Decode one UTF-8 scalar; *out=cp, return bytes (>=1), 0 on empty input. */
static size_t gw_decode(const char *s, size_t len, uint32_t *out) {
    const unsigned char *p = (const unsigned char *)s;
    uint32_t cp; size_t n, i;
    if (len == 0) { *out = 0; return 0; }
    if (p[0] < 0x80) { *out = p[0]; return 1; }
    else if ((p[0] & 0xE0) == 0xC0) { cp = p[0] & 0x1F; n = 2; }
    else if ((p[0] & 0xF0) == 0xE0) { cp = p[0] & 0x0F; n = 3; }
    else if ((p[0] & 0xF8) == 0xF0) { cp = p[0] & 0x07; n = 4; }
    else { *out = 0xFFFD; return 1; }
    if (n > len) { *out = 0xFFFD; return 1; }
    for (i = 1; i < n; i++) {
        if ((p[i] & 0xC0) != 0x80) { *out = 0xFFFD; return 1; }
        cp = (cp << 6) | (p[i] & 0x3F);
    }
    *out = cp;
    return n;
}

/* Advance over one grapheme cluster starting at s. */
int graphwidth_next(const char *s, size_t len, gw_cluster *cl) {
    uint32_t cp; size_t adv, off = 0;
    int cw;
    if (!s || !cl || len == 0) return -1;
    adv = gw_decode(s, len, &cp);
    if (adv == 0) return -1;
    cw = graphwidth_cp(cp);
    cl->columns = (cw < 0) ? 1 : cw;
    off = adv;
    /* Fold trailing zero-width marks, variation selectors and ZWJ runs. */
    while (off < len) {
        size_t n = gw_decode(s + off, len - off, &cp);
        if (n == 0) break;
        if (cp == GW_ZWJ) {
            off += n;
            if (off >= len) break;
            n = gw_decode(s + off, len - off, &cp);
            if (n == 0) break;
            off += n;            /* glue the joined scalar into this cluster */
            continue;
        }
        if (!graphwidth_is_zero(cp)) break;
        off += n;
    }
    cl->bytes = off;
    return 0;
}

/* Total display columns of a UTF-8 string. */
int graphwidth_string(const char *s, size_t len, int *columns) {
    size_t off = 0; int total = 0;
    gw_cluster cl;
    if (!s || !columns) return -1;
    while (off < len) {
        if (graphwidth_next(s + off, len - off, &cl) != 0) return -1;
        if (cl.bytes == 0) break;
        total += cl.columns;
        off += cl.bytes;
    }
    *columns = total;
    return 0;
}

/* Count grapheme clusters; write a summary into buf. */
int graphwidth_count(const char *s, size_t len, char *buf, size_t bufsz) {
    size_t off = 0; int clusters = 0, cols = 0;
    gw_cluster cl;
    if (!s || !buf || bufsz == 0) return -1;
    while (off < len) {
        if (graphwidth_next(s + off, len - off, &cl) != 0) return -1;
        if (cl.bytes == 0) break;
        clusters++;
        cols += cl.columns;
        off += cl.bytes;
    }
    if (snprintf(buf, bufsz, "clusters=%d cols=%d", clusters, cols) < 0)
        return -1;
    return clusters;
}
