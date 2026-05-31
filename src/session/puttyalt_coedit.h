#ifndef PUTTYALT_COEDIT_H
#define PUTTYALT_COEDIT_H

/*
 * puttyalt_coedit - real-time co-edit cursor tracking for shared sessions.
 *
 * Tracks the live cursor position (row/col) of each remote collaborator in a
 * shared terminal so the local renderer can draw "remote cursors" with name
 * labels and per-peer colours. Pure book-keeping: no I/O, static-sized state.
 */

#include <stdint.h>

#define COEDIT_MAX_PEERS     16
#define COEDIT_NAME_LEN      32
#define COEDIT_STALE_MS      4000   /* peers silent this long are dimmed */

typedef struct {
    uint32_t peer_id;                 /* 0 == empty slot */
    char     name[COEDIT_NAME_LEN];
    uint32_t color;                   /* 0xRRGGBB label colour */
    int      row;                     /* terminal cursor row */
    int      col;                     /* terminal cursor col */
    int      visible;                 /* peer wants cursor drawn */
    uint32_t last_update_ms;          /* monotonic ms of last move */
} CoeditPeer;

typedef struct {
    CoeditPeer peers[COEDIT_MAX_PEERS];
    int        peer_count;
    uint32_t   now_ms;                /* clock fed by coedit_tick */
} CoeditTracker;

void coedit_init(CoeditTracker *t);
void coedit_tick(CoeditTracker *t, uint32_t now_ms);

/* Returns slot index >= 0, or -1 if the roster is full. Re-joining an
 * existing peer_id updates its name/colour without consuming a new slot. */
int  coedit_join(CoeditTracker *t, uint32_t peer_id,
                 const char *name, uint32_t color);
int  coedit_leave(CoeditTracker *t, uint32_t peer_id);

/* Record a cursor move for a peer; auto-joins unknown peers if room. */
int  coedit_move(CoeditTracker *t, uint32_t peer_id, int row, int col);
int  coedit_set_visible(CoeditTracker *t, uint32_t peer_id, int visible);

CoeditPeer *coedit_find(CoeditTracker *t, uint32_t peer_id);
int  coedit_is_stale(const CoeditTracker *t, const CoeditPeer *p);

/* Count peers currently drawing a cursor on the given row (for the renderer). */
int  coedit_peers_on_row(const CoeditTracker *t, int row,
                         CoeditPeer **out, int max_out);

#endif /* PUTTYALT_COEDIT_H */
