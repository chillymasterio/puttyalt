#include "puttyalt_coedit.h"
#include <string.h>
#include <stdio.h>

void coedit_init(CoeditTracker *t)
{
    memset(t, 0, sizeof(*t));
}

void coedit_tick(CoeditTracker *t, uint32_t now_ms)
{
    t->now_ms = now_ms;
}

CoeditPeer *coedit_find(CoeditTracker *t, uint32_t peer_id)
{
    if (peer_id == 0) return NULL;
    for (int i = 0; i < t->peer_count; i++)
        if (t->peers[i].peer_id == peer_id)
            return &t->peers[i];
    return NULL;
}

int coedit_join(CoeditTracker *t, uint32_t peer_id,
                const char *name, uint32_t color)
{
    if (peer_id == 0) return -1;

    CoeditPeer *p = coedit_find(t, peer_id);
    if (!p) {
        if (t->peer_count >= COEDIT_MAX_PEERS) return -1;
        p = &t->peers[t->peer_count++];
        memset(p, 0, sizeof(*p));
        p->peer_id = peer_id;
        p->visible = 1;
    }
    if (name)
        snprintf(p->name, sizeof(p->name), "%s", name);
    p->color = color;
    p->last_update_ms = t->now_ms;
    return (int)(p - t->peers);
}

int coedit_leave(CoeditTracker *t, uint32_t peer_id)
{
    for (int i = 0; i < t->peer_count; i++) {
        if (t->peers[i].peer_id == peer_id) {
            for (int j = i; j < t->peer_count - 1; j++)
                t->peers[j] = t->peers[j + 1];
            t->peer_count--;
            memset(&t->peers[t->peer_count], 0, sizeof(CoeditPeer));
            return 0;
        }
    }
    return -1;
}

int coedit_move(CoeditTracker *t, uint32_t peer_id, int row, int col)
{
    CoeditPeer *p = coedit_find(t, peer_id);
    if (!p) {
        int idx = coedit_join(t, peer_id, NULL, 0);
        if (idx < 0) return -1;
        p = &t->peers[idx];
    }
    p->row = row;
    p->col = col;
    p->last_update_ms = t->now_ms;
    return 0;
}

int coedit_set_visible(CoeditTracker *t, uint32_t peer_id, int visible)
{
    CoeditPeer *p = coedit_find(t, peer_id);
    if (!p) return -1;
    p->visible = visible ? 1 : 0;
    return 0;
}

int coedit_is_stale(const CoeditTracker *t, const CoeditPeer *p)
{
    if (!p || p->peer_id == 0) return 1;
    return (t->now_ms - p->last_update_ms) > (uint32_t)COEDIT_STALE_MS;
}

int coedit_peers_on_row(const CoeditTracker *t, int row,
                        CoeditPeer **out, int max_out)
{
    int n = 0;
    for (int i = 0; i < t->peer_count; i++) {
        const CoeditPeer *p = &t->peers[i];
        if (p->peer_id == 0 || !p->visible || p->row != row) continue;
        if (out && n < max_out)
            out[n] = (CoeditPeer *)p;
        n++;
    }
    return n;
}
