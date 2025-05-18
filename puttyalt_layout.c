#include <string.h>
#include <stdio.h>
#include "puttyalt_layout.h"

void layout_init(LayoutManager *lm)
{
    memset(lm, 0, sizeof(*lm));
}

int layout_create(LayoutManager *lm, const char *name)
{
    if (lm->count >= LAYOUT_MAX_PRESETS) return -1;
    LayoutPreset *p = &lm->presets[lm->count];
    memset(p, 0, sizeof(*p));
    strncpy(p->name, name, LAYOUT_NAME_LEN - 1);
    return lm->count++;
}

int layout_add_pane(LayoutManager *lm, int preset_idx,
                    int x_pct, int y_pct, int w_pct, int h_pct)
{
    if (preset_idx < 0 || preset_idx >= lm->count) return -1;
    LayoutPreset *p = &lm->presets[preset_idx];
    if (p->num_panes >= LAYOUT_MAX_PANES) return -1;
    /* Validate percentages */
    if (x_pct + w_pct > 100 || y_pct + h_pct > 100) return -1;
    LayoutPane *lp = &p->panes[p->num_panes];
    lp->x_pct = x_pct;
    lp->y_pct = y_pct;
    lp->w_pct = w_pct;
    lp->h_pct = h_pct;
    lp->session_template = -1;
    p->num_panes++;
    return 0;
}

int layout_delete(LayoutManager *lm, int index)
{
    if (index < 0 || index >= lm->count) return -1;
    for (int i = index; i < lm->count - 1; i++)
        lm->presets[i] = lm->presets[i + 1];
    lm->count--;
    return 0;
}

int layout_apply(const LayoutManager *lm, int index)
{
    if (index < 0 || index >= lm->count) return -1;
    /* Would resize/reposition terminal panes */
    return 0;
}

int layout_save(const LayoutManager *lm, const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) return -1;
    for (int i = 0; i < lm->count; i++) {
        const LayoutPreset *p = &lm->presets[i];
        fprintf(fp, "[layout:%s]\npanes=%d\n", p->name, p->num_panes);
        for (int j = 0; j < p->num_panes; j++)
            fprintf(fp, "pane=%d,%d,%d,%d\n",
                    p->panes[j].x_pct, p->panes[j].y_pct,
                    p->panes[j].w_pct, p->panes[j].h_pct);
    }
    fclose(fp);
    return 0;
}

int layout_load(LayoutManager *lm, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    fclose(fp);
    return 0;
}
