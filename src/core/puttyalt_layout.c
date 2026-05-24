#include "puttyalt_layout.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void layout_init(LayoutMgr *lm)
{
    memset(lm, 0, sizeof(*lm));
    lm->active = -1;
    /* Default: single terminal pane */
    lm->current[0].type = PANE_TERMINAL;
    lm->current[0].x = 0;
    lm->current[0].y = 0;
    lm->current[0].w = 100;
    lm->current[0].h = 100;
    lm->current[0].focused = 1;
    lm->current_count = 1;
}

int layout_save(LayoutMgr *lm, const char *name)
{
    if (lm->count >= LAYOUT_MAX_SAVED) return -1;
    int idx = layout_find(lm, name);
    if (idx < 0) idx = lm->count++;
    SavedLayout *sl = &lm->layouts[idx];
    snprintf(sl->name, LAYOUT_MAX_NAME, "%s", name);
    memcpy(sl->panes, lm->current, sizeof(sl->panes));
    sl->pane_count = lm->current_count;
    return idx;
}

int layout_restore(LayoutMgr *lm, int index)
{
    if (index < 0 || index >= lm->count) return -1;
    const SavedLayout *sl = &lm->layouts[index];
    memcpy(lm->current, sl->panes, sizeof(lm->current));
    lm->current_count = sl->pane_count;
    lm->active = index;
    return 0;
}

int layout_find(const LayoutMgr *lm, const char *name)
{
    for (int i = 0; i < lm->count; i++)
        if (strcmp(lm->layouts[i].name, name) == 0) return i;
    return -1;
}

int layout_remove(LayoutMgr *lm, int index)
{
    if (index < 0 || index >= lm->count) return -1;
    for (int i = index; i < lm->count - 1; i++)
        lm->layouts[i] = lm->layouts[i + 1];
    lm->count--;
    return 0;
}

int layout_add_pane(LayoutMgr *lm, PaneType type, int x, int y, int w, int h)
{
    if (lm->current_count >= LAYOUT_MAX_PANES) return -1;
    LayoutPane *p = &lm->current[lm->current_count];
    p->type = type;
    p->x = x; p->y = y; p->w = w; p->h = h;
    p->session_id = -1;
    p->focused = 0;
    return lm->current_count++;
}

int layout_remove_pane(LayoutMgr *lm, int index)
{
    if (index < 0 || index >= lm->current_count) return -1;
    if (lm->current_count == 1) return -1; /* keep at least one */
    for (int i = index; i < lm->current_count - 1; i++)
        lm->current[i] = lm->current[i + 1];
    lm->current_count--;
    return 0;
}

int layout_resize_pane(LayoutMgr *lm, int index, int w, int h)
{
    if (index < 0 || index >= lm->current_count) return -1;
    lm->current[index].w = w;
    lm->current[index].h = h;
    return 0;
}

int layout_load_file(LayoutMgr *lm, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[256];
    SavedLayout *cur = NULL;
    if (!f) return -1;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[layout]") == 0) {
            if (lm->count >= LAYOUT_MAX_SAVED) break;
            cur = &lm->layouts[lm->count++];
            memset(cur, 0, sizeof(*cur));
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, LAYOUT_MAX_NAME, "%s", line + 5);
        else if (strncmp(line, "pane=", 5) == 0) {
            if (cur->pane_count < LAYOUT_MAX_PANES) {
                LayoutPane *p = &cur->panes[cur->pane_count++];
                sscanf(line + 5, "%d,%d,%d,%d,%d",
                       (int*)&p->type, &p->x, &p->y, &p->w, &p->h);
            }
        }
    }
    fclose(f);
    return 0;
}

int layout_save_file(const LayoutMgr *lm, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < lm->count; i++) {
        const SavedLayout *sl = &lm->layouts[i];
        fprintf(f, "[layout]\nname=%s\n", sl->name);
        for (int j = 0; j < sl->pane_count; j++) {
            const LayoutPane *p = &sl->panes[j];
            fprintf(f, "pane=%d,%d,%d,%d,%d\n",
                    p->type, p->x, p->y, p->w, p->h);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}
