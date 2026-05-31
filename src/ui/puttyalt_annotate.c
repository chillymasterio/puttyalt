/* puttyalt_annotate.c - shared annotation overlay for collaborative sessions.
 * Transient notes/highlights/pins anchored to terminal cells, authored by
 * peers and drawn above the grid; entries may auto-expire via a TTL clock.
 * Self-contained: all types are defined below. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ANNOT_MAX        64
#define ANNOT_TEXT_LEN   96

typedef enum {
    ANNOT_NOTE = 0,     /* free text bubble */
    ANNOT_HIGHLIGHT,    /* coloured span on one row */
    ANNOT_PIN           /* single-cell marker */
} AnnotKind;

typedef struct {
    uint32_t  id;                 /* 0 == empty slot */
    AnnotKind kind;
    int       row, col, span;     /* span: cells covered (HIGHLIGHT); >=1 */
    char      text[ANNOT_TEXT_LEN];
    uint32_t  author_id;
    uint32_t  color;              /* 0xRRGGBB */
    uint32_t  created_ms;
    uint32_t  expire_ms;          /* 0 == never expires */
} Annotation;

typedef struct {
    Annotation items[ANNOT_MAX];
    int        count;
    uint32_t   next_id;
    uint32_t   now_ms;
} AnnotOverlay;

void annot_init(AnnotOverlay *o)
{
    if (!o) return;
    memset(o, 0, sizeof(*o));
    o->next_id = 1;
}

static void annot_remove_index(AnnotOverlay *o, int i)
{
    for (int j = i; j < o->count - 1; j++)
        o->items[j] = o->items[j + 1];
    o->count--;
    memset(&o->items[o->count], 0, sizeof(Annotation));
}

/* Drop expired annotations against the supplied clock. */
void annot_tick(AnnotOverlay *o, uint32_t now_ms)
{
    if (!o) return;
    o->now_ms = now_ms;
    for (int i = 0; i < o->count; ) {
        Annotation *a = &o->items[i];
        if (a->expire_ms != 0 && now_ms >= a->expire_ms)
            annot_remove_index(o, i);
        else
            i++;
    }
}

/* Add an annotation; returns its new id (>0), or 0 on failure.
 * ttl_ms == 0 means it never auto-expires. */
uint32_t annot_add(AnnotOverlay *o, AnnotKind kind, int row, int col,
                   int span, const char *text, uint32_t author_id,
                   uint32_t color, uint32_t ttl_ms)
{
    if (!o || o->count >= ANNOT_MAX || row < 0 || col < 0) return 0;
    if (span < 1) span = 1;

    Annotation *a = &o->items[o->count++];
    memset(a, 0, sizeof(*a));
    a->id = o->next_id++;
    if (o->next_id == 0) o->next_id = 1;   /* skip the sentinel on wrap */
    a->kind = kind;
    a->row = row;
    a->col = col;
    a->span = span;
    if (text)
        snprintf(a->text, sizeof(a->text), "%s", text);
    a->author_id = author_id;
    a->color = color;
    a->created_ms = o->now_ms;
    a->expire_ms = ttl_ms ? (o->now_ms + ttl_ms) : 0;
    return a->id;
}

Annotation *annot_find(AnnotOverlay *o, uint32_t id)
{
    if (!o || id == 0) return NULL;
    for (int i = 0; i < o->count; i++)
        if (o->items[i].id == id)
            return &o->items[i];
    return NULL;
}

int annot_remove(AnnotOverlay *o, uint32_t id)
{
    if (!o) return -1;
    for (int i = 0; i < o->count; i++)
        if (o->items[i].id == id) {
            annot_remove_index(o, i);
            return 0;
        }
    return -1;
}

int annot_edit_text(AnnotOverlay *o, uint32_t id, const char *text)
{
    Annotation *a = annot_find(o, id);
    if (!a || !text) return -1;
    snprintf(a->text, sizeof(a->text), "%s", text);
    return 0;
}

/* Remove every annotation authored by a peer (e.g. on leave). */
int annot_clear_author(AnnotOverlay *o, uint32_t author_id)
{
    if (!o) return -1;
    int removed = 0;
    for (int i = 0; i < o->count; ) {
        if (o->items[i].author_id == author_id) {
            annot_remove_index(o, i);
            removed++;
        } else {
            i++;
        }
    }
    return removed;
}

/* Collect annotations on a given row into out[] (for the renderer);
 * returns the total found, even if it exceeds max_out. */
int annot_on_row(const AnnotOverlay *o, int row,
                 Annotation **out, int max_out)
{
    if (!o) return -1;
    int n = 0;
    for (int i = 0; i < o->count; i++) {
        const Annotation *a = &o->items[i];
        if (a->id == 0 || a->row != row) continue;
        if (out && n < max_out)
            out[n] = (Annotation *)a;
        n++;
    }
    return n;
}
