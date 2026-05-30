/*
 * puttyalt_panecast.c - multi-pane broadcast for PuttyAlt.
 * Fans out a keystroke buffer to all active members of a pane group.
 * Self-contained library module. No project headers, no windows.h.
 */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define PANECAST_MAX_PANES   64
#define PANECAST_MAX_DISPATCH 4096

typedef struct PaneCastMember {
    int      pane_id;   /* caller-assigned identifier, must be >= 0 */
    int      active;    /* nonzero when this slot is in use         */
    uint32_t sent;      /* number of dispatches delivered here      */
} PaneCastMember;

typedef struct PaneCastGroup {
    PaneCastMember members[PANECAST_MAX_PANES];
    int            count;    /* number of active members            */
    int            enabled;  /* broadcast on/off toggle             */
} PaneCastGroup;

void panecast_init(PaneCastGroup *g)
{
    if (g == NULL)
        return;
    memset(g, 0, sizeof(*g));
    g->count = 0;
    g->enabled = 0;
}

/* Locate an active member by id, or -1 if not present. */
static int panecast_find(const PaneCastGroup *g, int pane_id)
{
    int i;
    for (i = 0; i < PANECAST_MAX_PANES; i++) {
        if (g->members[i].active && g->members[i].pane_id == pane_id)
            return i;
    }
    return -1;
}

int panecast_add(PaneCastGroup *g, int pane_id)
{
    int i, free_slot = -1;
    if (g == NULL || pane_id < 0)
        return -1;
    if (panecast_find(g, pane_id) >= 0)
        return -1;                      /* duplicate */
    for (i = 0; i < PANECAST_MAX_PANES; i++) {
        if (!g->members[i].active) {
            free_slot = i;
            break;
        }
    }
    if (free_slot < 0)
        return -1;                      /* group full */
    g->members[free_slot].pane_id = pane_id;
    g->members[free_slot].active = 1;
    g->members[free_slot].sent = 0;
    g->count++;
    return 0;
}

int panecast_remove(PaneCastGroup *g, int pane_id)
{
    int idx;
    if (g == NULL || pane_id < 0)
        return -1;
    idx = panecast_find(g, pane_id);
    if (idx < 0)
        return -1;
    g->members[idx].active = 0;
    g->members[idx].pane_id = 0;
    g->members[idx].sent = 0;
    if (g->count > 0)
        g->count--;
    return 0;
}

void panecast_set_enabled(PaneCastGroup *g, int enabled)
{
    if (g == NULL)
        return;
    g->enabled = enabled ? 1 : 0;
}

/* Fan out to active members; record receiving pane ids into out_targets
 * (up to cap). Returns number of targets, or -1 on error. */
int panecast_dispatch(const PaneCastGroup *g, const char *data, int len,
                      int *out_targets, int cap)
{
    int i, n = 0;
    if (g == NULL || data == NULL || len < 0)
        return -1;
    if (len > PANECAST_MAX_DISPATCH)
        return -1;
    if (!g->enabled || len == 0)
        return 0;
    for (i = 0; i < PANECAST_MAX_PANES; i++) {
        if (!g->members[i].active)
            continue;
        if (out_targets != NULL && n < cap)
            out_targets[n] = g->members[i].pane_id;
        n++;
    }
    return n;
}
