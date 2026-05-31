/* puttyalt_lineundo.c - Undo/redo ring for line-mode terminal edits. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LINEUNDO_MAX_LEN   256u
#define LINEUNDO_CAP       32u

typedef struct {
    char     text[LINEUNDO_MAX_LEN];
    uint32_t len;
    uint32_t cursor;
    uint64_t ts_ms;
} lineundo_snap_t;

typedef struct {
    lineundo_snap_t snaps[LINEUNDO_CAP];
    uint32_t        count;   /* live snapshots in ring                 */
    uint32_t        head;    /* ring index of oldest live snapshot     */
    uint32_t        cur;     /* logical index [0,count) of current pos */
} lineundo_t;

static lineundo_snap_t *lineundo_at(lineundo_t *u, uint32_t logical) {
    return &u->snaps[(u->head + logical) % LINEUNDO_CAP];
}

void lineundo_init(lineundo_t *u) {
    if (!u) return;
    memset(u, 0, sizeof(*u));
}

void lineundo_reset(lineundo_t *u) {
    lineundo_init(u);
}

uint32_t lineundo_depth(const lineundo_t *u) {
    return u ? u->count : 0u;
}

int lineundo_can_undo(const lineundo_t *u) {
    return (u && u->count > 0 && u->cur > 0) ? 1 : 0;
}

int lineundo_can_redo(const lineundo_t *u) {
    return (u && u->count > 0 && u->cur + 1u < u->count) ? 1 : 0;
}

int lineundo_record(lineundo_t *u, const char *text, uint32_t cursor,
                    uint64_t now_ms) {
    lineundo_snap_t *s;
    uint32_t n;
    if (!u || !text) return -1;
    n = (uint32_t)strnlen(text, LINEUNDO_MAX_LEN - 1u);
    if (cursor > n) cursor = n;

    /* No-op suppression: identical to current snapshot. */
    if (u->count > 0) {
        s = lineundo_at(u, u->cur);
        if (s->len == n && s->cursor == cursor &&
            memcmp(s->text, text, n) == 0)
            return 0;
        /* Truncate any redo branch after the current position. */
        u->count = u->cur + 1u;
    }

    if (u->count >= LINEUNDO_CAP) {
        /* Evict oldest snapshot to make room. */
        u->head = (u->head + 1u) % LINEUNDO_CAP;
        u->count--;
        if (u->cur > 0) u->cur--;
    }

    s = lineundo_at(u, u->count);
    memcpy(s->text, text, n);
    s->text[n] = '\0';
    s->len = n;
    s->cursor = cursor;
    s->ts_ms = now_ms;
    u->count++;
    u->cur = u->count - 1u;
    return 0;
}

int lineundo_undo(lineundo_t *u, char *out, uint32_t out_sz,
                  uint32_t *cursor_out) {
    lineundo_snap_t *s;
    if (!lineundo_can_undo(u) || !out || out_sz == 0) return -1;
    u->cur--;
    s = lineundo_at(u, u->cur);
    snprintf(out, out_sz, "%s", s->text);
    if (cursor_out) *cursor_out = s->cursor;
    return 0;
}

int lineundo_redo(lineundo_t *u, char *out, uint32_t out_sz,
                  uint32_t *cursor_out) {
    lineundo_snap_t *s;
    if (!lineundo_can_redo(u) || !out || out_sz == 0) return -1;
    u->cur++;
    s = lineundo_at(u, u->cur);
    snprintf(out, out_sz, "%s", s->text);
    if (cursor_out) *cursor_out = s->cursor;
    return 0;
}
