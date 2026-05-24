#ifndef PUTTYALT_MUX_H
#define PUTTYALT_MUX_H

#define MUX_MAX_PANES  8
#define MUX_MAX_LAYOUTS 4

typedef enum {
    MUX_SPLIT_NONE,
    MUX_SPLIT_HORIZ,
    MUX_SPLIT_VERT
} MuxSplitDir;

typedef struct {
    int id;
    int x, y, w, h;    /* percentage-based 0-100 */
    int session_id;
    int active;
    int scrollback_pos;
    char title[128];
} MuxPane;

typedef struct {
    MuxPane panes[MUX_MAX_PANES];
    int num_panes;
    int active_pane;
    int focus_pane;
    MuxSplitDir last_split;
} TermMux;

void mux_init(TermMux *m);
int  mux_split(TermMux *m, int pane_id, MuxSplitDir dir);
int  mux_close_pane(TermMux *m, int pane_id);
void mux_focus_next(TermMux *m);
void mux_focus_prev(TermMux *m);
void mux_focus_pane(TermMux *m, int pane_id);
int  mux_resize_pane(TermMux *m, int pane_id, int delta);
void mux_equalize(TermMux *m);
MuxPane *mux_get_active(TermMux *m);

#endif
