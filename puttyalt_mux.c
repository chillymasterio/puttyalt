#include "puttyalt_mux.h"
#include <string.h>
#include <stdio.h>

void mux_init(TermMux *m)
{
    memset(m, 0, sizeof(*m));
    MuxPane *p = &m->panes[0];
    p->id = 0;
    p->x = 0; p->y = 0; p->w = 100; p->h = 100;
    p->active = 1;
    snprintf(p->title, sizeof(p->title), "Pane 1");
    m->num_panes = 1;
    m->active_pane = 0;
    m->focus_pane = 0;
}

int mux_split(TermMux *m, int pane_id, MuxSplitDir dir)
{
    if (m->num_panes >= MUX_MAX_PANES) return -1;
    if (pane_id < 0 || pane_id >= m->num_panes) return -1;

    MuxPane *src = &m->panes[pane_id];
    MuxPane *dst = &m->panes[m->num_panes];
    memset(dst, 0, sizeof(*dst));
    dst->id = m->num_panes;
    dst->active = 1;
    snprintf(dst->title, sizeof(dst->title), "Pane %d", m->num_panes + 1);

    if (dir == MUX_SPLIT_HORIZ) {
        int half = src->h / 2;
        dst->x = src->x; dst->y = src->y + half;
        dst->w = src->w; dst->h = src->h - half;
        src->h = half;
    } else {
        int half = src->w / 2;
        dst->x = src->x + half; dst->y = src->y;
        dst->w = src->w - half; dst->h = src->h;
        src->w = half;
    }

    m->num_panes++;
    m->last_split = dir;
    return dst->id;
}

int mux_close_pane(TermMux *m, int pane_id)
{
    if (m->num_panes <= 1) return -1;
    if (pane_id < 0 || pane_id >= m->num_panes) return -1;

    for (int i = pane_id; i < m->num_panes - 1; i++) {
        m->panes[i] = m->panes[i + 1];
        m->panes[i].id = i;
    }
    m->num_panes--;

    if (m->focus_pane >= m->num_panes)
        m->focus_pane = m->num_panes - 1;
    if (m->active_pane >= m->num_panes)
        m->active_pane = m->num_panes - 1;

    /* Give first pane remaining space */
    if (m->num_panes == 1) {
        m->panes[0].x = 0; m->panes[0].y = 0;
        m->panes[0].w = 100; m->panes[0].h = 100;
    }
    return 0;
}

void mux_focus_next(TermMux *m)
{
    if (m->num_panes <= 1) return;
    m->focus_pane = (m->focus_pane + 1) % m->num_panes;
}

void mux_focus_prev(TermMux *m)
{
    if (m->num_panes <= 1) return;
    m->focus_pane = (m->focus_pane - 1 + m->num_panes) % m->num_panes;
}

void mux_focus_pane(TermMux *m, int pane_id)
{
    if (pane_id >= 0 && pane_id < m->num_panes)
        m->focus_pane = pane_id;
}

int mux_resize_pane(TermMux *m, int pane_id, int delta)
{
    if (pane_id < 0 || pane_id >= m->num_panes) return -1;
    MuxPane *p = &m->panes[pane_id];
    int new_w = p->w + delta;
    if (new_w < 10 || new_w > 90) return -1;
    p->w = new_w;
    return 0;
}

void mux_equalize(TermMux *m)
{
    if (m->num_panes <= 0) return;
    int w = 100 / m->num_panes;
    for (int i = 0; i < m->num_panes; i++) {
        m->panes[i].x = i * w;
        m->panes[i].y = 0;
        m->panes[i].w = w;
        m->panes[i].h = 100;
    }
}

MuxPane *mux_get_active(TermMux *m)
{
    if (m->focus_pane >= 0 && m->focus_pane < m->num_panes)
        return &m->panes[m->focus_pane];
    return NULL;
}
