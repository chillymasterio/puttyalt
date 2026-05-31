/* puttyalt_glyphatlas.c - GPU texture atlas for glyphs (shelf packing + LRU). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define GLYPHATLAS_DIM 1024
#define GLYPHATLAS_MAX_GLYPHS 256
#define GLYPHATLAS_MAX_SHELVES 64

typedef struct {
    uint32_t codepoint;
    uint16_t style;
    uint16_t size;
} glyphatlas_key_t;

typedef struct {
    glyphatlas_key_t key;
    uint16_t x, y, w, h;
    uint64_t last_used_ms;
    int in_use;
} glyphatlas_glyph_t;

typedef struct {
    uint16_t y, height, cursor_x;
    int active;
} glyphatlas_shelf_t;

typedef struct {
    glyphatlas_glyph_t glyphs[GLYPHATLAS_MAX_GLYPHS];
    glyphatlas_shelf_t shelves[GLYPHATLAS_MAX_SHELVES];
    int glyph_count, shelf_count;
    uint32_t used_area;
} glyphatlas_t;

static glyphatlas_t g_atlas;

static int glyphatlas_key_eq(const glyphatlas_key_t *a, const glyphatlas_key_t *b) {
    return a->codepoint == b->codepoint && a->style == b->style && a->size == b->size;
}

void glyphatlas_init(void) {
    memset(&g_atlas, 0, sizeof(g_atlas));
}

void glyphatlas_reset(void) {
    glyphatlas_init();
}

int glyphatlas_lookup(const glyphatlas_key_t *key, uint64_t now_ms) {
    int i;
    if (!key) return -1;
    for (i = 0; i < g_atlas.glyph_count; i++) {
        if (g_atlas.glyphs[i].in_use && glyphatlas_key_eq(&g_atlas.glyphs[i].key, key)) {
            g_atlas.glyphs[i].last_used_ms = now_ms;
            return i;
        }
    }
    return -1;
}

int glyphatlas_evict_lru(void) {
    int i, victim = -1;
    uint64_t oldest = UINT64_MAX;
    for (i = 0; i < g_atlas.glyph_count; i++) {
        if (g_atlas.glyphs[i].in_use && g_atlas.glyphs[i].last_used_ms <= oldest) {
            oldest = g_atlas.glyphs[i].last_used_ms;
            victim = i;
        }
    }
    if (victim < 0) return -1;
    g_atlas.used_area -= (uint32_t)g_atlas.glyphs[victim].w * g_atlas.glyphs[victim].h;
    g_atlas.glyphs[victim].in_use = 0;
    return victim;
}

static int glyphatlas_place(uint16_t w, uint16_t h, uint16_t *ox, uint16_t *oy) {
    int i;
    for (i = 0; i < g_atlas.shelf_count; i++) {
        glyphatlas_shelf_t *s = &g_atlas.shelves[i];
        if (s->active && h <= s->height && (uint32_t)s->cursor_x + w <= GLYPHATLAS_DIM) {
            *ox = s->cursor_x; *oy = s->y; s->cursor_x = (uint16_t)(s->cursor_x + w);
            return 0;
        }
    }
    uint16_t top = 0;
    for (i = 0; i < g_atlas.shelf_count; i++)
        top = (uint16_t)(top > g_atlas.shelves[i].y + g_atlas.shelves[i].height ?
                         top : g_atlas.shelves[i].y + g_atlas.shelves[i].height);
    if (g_atlas.shelf_count >= GLYPHATLAS_MAX_SHELVES) return -1;
    if ((uint32_t)top + h > GLYPHATLAS_DIM) return -1;
    glyphatlas_shelf_t *s = &g_atlas.shelves[g_atlas.shelf_count++];
    s->y = top; s->height = h; s->cursor_x = w; s->active = 1;
    *ox = 0; *oy = top;
    return 0;
}

int glyphatlas_insert(const glyphatlas_key_t *key, uint16_t w, uint16_t h, uint64_t now_ms) {
    int slot = -1, i;
    uint16_t x, y;
    if (!key || w == 0 || h == 0 || w > GLYPHATLAS_DIM || h > GLYPHATLAS_DIM) return -1;
    if (glyphatlas_lookup(key, now_ms) >= 0) return -1;
    if (glyphatlas_place(w, h, &x, &y) != 0) {
        if (glyphatlas_evict_lru() < 0) return -1;
        if (glyphatlas_place(w, h, &x, &y) != 0) return -1;
    }
    for (i = 0; i < g_atlas.glyph_count; i++)
        if (!g_atlas.glyphs[i].in_use) { slot = i; break; }
    if (slot < 0) {
        if (g_atlas.glyph_count >= GLYPHATLAS_MAX_GLYPHS) return -1;
        slot = g_atlas.glyph_count++;
    }
    g_atlas.glyphs[slot].key = *key;
    g_atlas.glyphs[slot].x = x; g_atlas.glyphs[slot].y = y;
    g_atlas.glyphs[slot].w = w; g_atlas.glyphs[slot].h = h;
    g_atlas.glyphs[slot].last_used_ms = now_ms;
    g_atlas.glyphs[slot].in_use = 1;
    g_atlas.used_area += (uint32_t)w * h;
    return slot;
}

int glyphatlas_uv_rect(int index, float *u0, float *v0, float *u1, float *v1) {
    const glyphatlas_glyph_t *g;
    if (index < 0 || index >= g_atlas.glyph_count || !u0 || !v0 || !u1 || !v1) return -1;
    g = &g_atlas.glyphs[index];
    if (!g->in_use) return -1;
    *u0 = (float)g->x / GLYPHATLAS_DIM;
    *v0 = (float)g->y / GLYPHATLAS_DIM;
    *u1 = (float)(g->x + g->w) / GLYPHATLAS_DIM;
    *v1 = (float)(g->y + g->h) / GLYPHATLAS_DIM;
    return 0;
}

float glyphatlas_utilization(void) {
    return (float)g_atlas.used_area / ((float)GLYPHATLAS_DIM * GLYPHATLAS_DIM);
}

int glyphatlas_describe(char *buf, size_t cap) {
    int live = 0, i;
    if (!buf || cap == 0) return -1;
    for (i = 0; i < g_atlas.glyph_count; i++)
        if (g_atlas.glyphs[i].in_use) live++;
    return snprintf(buf, cap, "atlas %ux%u glyphs=%d shelves=%d util=%.1f%%",
                    GLYPHATLAS_DIM, GLYPHATLAS_DIM, live, g_atlas.shelf_count,
                    glyphatlas_utilization() * 100.0f);
}
