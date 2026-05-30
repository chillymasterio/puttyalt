/* puttyalt_layoutstore.c - named window/pane layout snapshot store */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LAY_MAX_NAME   32
#define LAY_MAX_RECTS  16
#define LAY_MAX_SNAPS  16

typedef struct {
    int32_t x, y, w, h;
} LayoutRect;

typedef struct {
    char name[LAY_MAX_NAME];
    LayoutRect rects[LAY_MAX_RECTS];
    int rect_count;
    int used;
} LayoutSnap;

typedef struct {
    LayoutSnap snaps[LAY_MAX_SNAPS];
    int count;
} LayoutStore;

void laystore_init(LayoutStore *ls)
{
    if (!ls) return;
    memset(ls, 0, sizeof(*ls));
}

static LayoutSnap *laystore_find(LayoutStore *ls, const char *name)
{
    int i;
    if (!ls || !name) return NULL;
    for (i = 0; i < LAY_MAX_SNAPS; i++)
        if (ls->snaps[i].used && strcmp(ls->snaps[i].name, name) == 0)
            return &ls->snaps[i];
    return NULL;
}

int laystore_capture(LayoutStore *ls, const char *name,
                     const LayoutRect *rects, int rect_count)
{
    LayoutSnap *s;
    int i;
    if (!ls || !name || !rects) return -1;
    if (name[0] == '\0' || strlen(name) >= LAY_MAX_NAME) return -1;
    if (rect_count < 0 || rect_count > LAY_MAX_RECTS) return -1;

    s = laystore_find(ls, name);
    if (!s) {
        for (i = 0; i < LAY_MAX_SNAPS; i++) {
            if (!ls->snaps[i].used) { s = &ls->snaps[i]; break; }
        }
        if (!s) return -1;
        memset(s, 0, sizeof(*s));
        memcpy(s->name, name, strlen(name) + 1);
        s->used = 1;
        ls->count++;
    }
    for (i = 0; i < rect_count; i++)
        s->rects[i] = rects[i];
    s->rect_count = rect_count;
    return 0;
}

int laystore_save_buf(const LayoutStore *ls, const char *name,
                     char *out, int cap)
{
    const LayoutSnap *s = laystore_find((LayoutStore *)ls, name);
    int i, off = 0, n;
    if (!ls || !out || cap <= 0) return -1;
    if (!s) return -1;
    n = snprintf(out, (size_t)cap, "[layout:%s]\ncount=%d\n", s->name, s->rect_count);
    if (n < 0 || n >= cap) return -1;
    off = n;
    for (i = 0; i < s->rect_count; i++) {
        n = snprintf(out + off, (size_t)(cap - off),
                     "r%d=%ld,%ld,%ld,%ld\n", i,
                     (long)s->rects[i].x, (long)s->rects[i].y,
                     (long)s->rects[i].w, (long)s->rects[i].h);
        if (n < 0 || n >= cap - off) return -1;
        off += n;
    }
    return off;
}

int laystore_load_buf(LayoutStore *ls, const char *name, const char *buf)
{
    LayoutRect rects[LAY_MAX_RECTS];
    int count = 0, idx;
    long x, y, w, h;
    const char *p;
    if (!ls || !name || !buf) return -1;
    p = strstr(buf, "count=");
    if (!p) return -1;
    if (sscanf(p, "count=%d", &count) != 1) return -1;
    if (count < 0 || count > LAY_MAX_RECTS) return -1;
    memset(rects, 0, sizeof(rects));
    p = buf;
    while ((p = strchr(p, 'r')) != NULL) {
        if (sscanf(p, "r%d=%ld,%ld,%ld,%ld", &idx, &x, &y, &w, &h) == 5 &&
            idx >= 0 && idx < count) {
            rects[idx].x = (int32_t)x; rects[idx].y = (int32_t)y;
            rects[idx].w = (int32_t)w; rects[idx].h = (int32_t)h;
        }
        p++;
    }
    return laystore_capture(ls, name, rects, count);
}

int laystore_list(const LayoutStore *ls, char *out, int cap)
{
    int i, off = 0, n;
    if (!ls || !out || cap <= 0) return -1;
    out[0] = '\0';
    for (i = 0; i < LAY_MAX_SNAPS; i++) {
        if (!ls->snaps[i].used) continue;
        n = snprintf(out + off, (size_t)(cap - off), "%s\n", ls->snaps[i].name);
        if (n < 0 || n >= cap - off) return -1;
        off += n;
    }
    return off;
}

int laystore_delete(LayoutStore *ls, const char *name)
{
    LayoutSnap *s = laystore_find(ls, name);
    if (!s) return -1;
    memset(s, 0, sizeof(*s));
    if (ls->count > 0) ls->count--;
    return 0;
}
