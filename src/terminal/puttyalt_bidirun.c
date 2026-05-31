/* puttyalt_bidirun.c - BiDi run segmentation for PuttyAlt terminal lines. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BIDIRUN_MAX_RUNS 256

typedef enum { BIDI_DIR_LTR = 0, BIDI_DIR_RTL = 1, BIDI_DIR_NEUTRAL = 2 } bidi_dir_t;

typedef struct {
    int      start_cp;   /* code-point offset of run start */
    int      length;     /* number of code points in run */
    bidi_dir_t dir;      /* resolved run direction (never neutral) */
} bidirun_t;

typedef struct {
    bidirun_t  runs[BIDIRUN_MAX_RUNS];
    int        count;
    bidi_dir_t base_dir;
    int        total_cp;
} bidirun_ctx_t;

/* Decode one UTF-8 code point; returns bytes consumed, *cp set, 0 on error. */
static int bidi_utf8_next(const uint8_t *s, size_t len, uint32_t *cp) {
    if (len == 0) return 0;
    if (s[0] < 0x80) { *cp = s[0]; return 1; }
    if ((s[0] & 0xE0) == 0xC0 && len >= 2 && (s[1] & 0xC0) == 0x80) {
        *cp = ((uint32_t)(s[0] & 0x1F) << 6) | (s[1] & 0x3F); return 2;
    }
    if ((s[0] & 0xF0) == 0xE0 && len >= 3 && (s[1] & 0xC0) == 0x80 && (s[2] & 0xC0) == 0x80) {
        *cp = ((uint32_t)(s[0] & 0x0F) << 12) | ((uint32_t)(s[1] & 0x3F) << 6) | (s[2] & 0x3F); return 3;
    }
    if ((s[0] & 0xF8) == 0xF0 && len >= 4 && (s[1] & 0xC0) == 0x80 && (s[2] & 0xC0) == 0x80 && (s[3] & 0xC0) == 0x80) {
        *cp = ((uint32_t)(s[0] & 0x07) << 18) | ((uint32_t)(s[1] & 0x3F) << 12) |
              ((uint32_t)(s[2] & 0x3F) << 6) | (s[3] & 0x3F); return 4;
    }
    *cp = 0xFFFD; return 1;
}

/* Strong/neutral classification over Hebrew/Arabic and Latin/CJK ranges. */
static bidi_dir_t bidi_classify(uint32_t cp) {
    if ((cp >= 0x0590 && cp <= 0x05FF) ||   /* Hebrew */
        (cp >= 0x0600 && cp <= 0x06FF) ||   /* Arabic */
        (cp >= 0x0700 && cp <= 0x074F) ||   /* Syriac */
        (cp >= 0xFB1D && cp <= 0xFB4F) ||   /* Hebrew presentation */
        (cp >= 0xFB50 && cp <= 0xFEFC))     /* Arabic presentation */
        return BIDI_DIR_RTL;
    if ((cp >= 0x0041 && cp <= 0x024F) ||   /* Latin */
        (cp >= 0x0370 && cp <= 0x03FF) ||   /* Greek */
        (cp >= 0x0400 && cp <= 0x04FF) ||   /* Cyrillic */
        (cp >= 0x3040 && cp <= 0x30FF) ||   /* Kana */
        (cp >= 0x4E00 && cp <= 0x9FFF))     /* CJK */
        return BIDI_DIR_LTR;
    return BIDI_DIR_NEUTRAL;
}

int bidirun_init(bidirun_ctx_t *ctx) {
    if (!ctx) return -1;
    memset(ctx, 0, sizeof(*ctx));
    ctx->base_dir = BIDI_DIR_LTR;
    return 0;
}

/* Base direction from first strong code point (P2/P3 simplified). */
bidi_dir_t bidirun_base_dir(const char *utf8, size_t len) {
    size_t i = 0; uint32_t cp;
    if (!utf8) return BIDI_DIR_LTR;
    while (i < len) {
        int n = bidi_utf8_next((const uint8_t *)utf8 + i, len - i, &cp);
        if (n <= 0) break;
        bidi_dir_t d = bidi_classify(cp);
        if (d != BIDI_DIR_NEUTRAL) return d;
        i += (size_t)n;
    }
    return BIDI_DIR_LTR;
}

int bidirun_segment(bidirun_ctx_t *ctx, const char *utf8, size_t len) {
    size_t i = 0; int cpidx = 0; uint32_t cp;
    bidi_dir_t cur = BIDI_DIR_NEUTRAL; int rstart = 0;
    if (!ctx || !utf8) return -1;
    ctx->count = 0; ctx->total_cp = 0;
    ctx->base_dir = bidirun_base_dir(utf8, len);
    while (i < len) {
        int n = bidi_utf8_next((const uint8_t *)utf8 + i, len - i, &cp);
        if (n <= 0) break;
        bidi_dir_t d = bidi_classify(cp);
        if (d == BIDI_DIR_NEUTRAL) d = ctx->base_dir; /* neutrals take base dir */
        if (d != cur) {
            if (cur != BIDI_DIR_NEUTRAL) {
                if (ctx->count >= BIDIRUN_MAX_RUNS) return -1;
                ctx->runs[ctx->count].start_cp = rstart;
                ctx->runs[ctx->count].length   = cpidx - rstart;
                ctx->runs[ctx->count].dir      = cur;
                ctx->count++;
            }
            cur = d; rstart = cpidx;
        }
        cpidx++; i += (size_t)n;
    }
    if (cur != BIDI_DIR_NEUTRAL && cpidx > rstart) {
        if (ctx->count >= BIDIRUN_MAX_RUNS) return -1;
        ctx->runs[ctx->count].start_cp = rstart;
        ctx->runs[ctx->count].length   = cpidx - rstart;
        ctx->runs[ctx->count].dir      = cur;
        ctx->count++;
    }
    ctx->total_cp = cpidx;
    return 0;
}

int bidirun_count(const bidirun_ctx_t *ctx) {
    return ctx ? ctx->count : -1;
}

int bidirun_get(const bidirun_ctx_t *ctx, int index, bidirun_t *out) {
    if (!ctx || !out || index < 0 || index >= ctx->count) return -1;
    *out = ctx->runs[index];
    return 0;
}

int bidirun_has_rtl(const bidirun_ctx_t *ctx) {
    int i;
    if (!ctx) return -1;
    for (i = 0; i < ctx->count; i++)
        if (ctx->runs[i].dir == BIDI_DIR_RTL) return 1;
    return 0;
}
