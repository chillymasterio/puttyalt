#include "puttyalt_selection.h"
#include <stdlib.h>
#include <string.h>

void sel_init(Selection *sel) { memset(sel, 0, sizeof(*sel)); }

void sel_destroy(Selection *sel) { free(sel->clipboard); sel->clipboard = NULL; }

void sel_start(Selection *sel, int row, int col, SelectMode mode)
{
    sel->start_row = sel->end_row = sel->anchor_row = row;
    sel->start_col = sel->end_col = sel->anchor_col = col;
    sel->mode = mode;
    sel->active = 1;
}

void sel_update(Selection *sel, int row, int col)
{
    if (!sel->active) return;
    sel->end_row = row;
    sel->end_col = col;
}

static void sel_normalize(Selection *sel, int *sr, int *sc, int *er, int *ec)
{
    if (sel->start_row < sel->end_row || (sel->start_row == sel->end_row && sel->start_col <= sel->end_col)) {
        *sr = sel->start_row; *sc = sel->start_col;
        *er = sel->end_row; *ec = sel->end_col;
    } else {
        *sr = sel->end_row; *sc = sel->end_col;
        *er = sel->start_row; *ec = sel->start_col;
    }
}

void sel_finish(Selection *sel, TermState *ts)
{
    sel->active = 0;
    int sr, sc, er, ec;
    sel_normalize(sel, &sr, &sc, &er, &ec);

    free(sel->clipboard);
    int maxlen = (er - sr + 1) * (ts->cols + 1) + 1;
    sel->clipboard = malloc(maxlen);
    if (!sel->clipboard) return;

    int pos = 0;
    for (int r = sr; r <= er; r++) {
        int c_start = (r == sr) ? sc : 0;
        int c_end = (r == er) ? ec : ts->cols - 1;
        if (sel->mode == SEL_LINE) { c_start = 0; c_end = ts->cols - 1; }
        for (int c = c_start; c <= c_end && c < ts->cols; c++) {
            TermCell *cell = term_cell_at(ts, r, c);
            if (cell && cell->ch >= 0x20 && cell->ch < 0x7F)
                sel->clipboard[pos++] = (char)cell->ch;
            else
                sel->clipboard[pos++] = ' ';
        }
        /* Trim trailing spaces */
        while (pos > 0 && sel->clipboard[pos - 1] == ' ') pos--;
        if (r < er) sel->clipboard[pos++] = '\n';
    }
    sel->clipboard[pos] = '\0';
    sel->clipboard_len = pos;
}

void sel_clear(Selection *sel)
{
    sel->active = 0;
    sel->mode = SEL_NONE;
    sel->start_row = sel->start_col = sel->end_row = sel->end_col = 0;
}

int sel_is_selected(Selection *sel, int row, int col)
{
    if (sel->mode == SEL_NONE) return 0;
    int sr, sc, er, ec;
    sel_normalize(sel, &sr, &sc, &er, &ec);
    if (sel->mode == SEL_LINE) return row >= sr && row <= er;
    if (sel->mode == SEL_RECT) return row >= sr && row <= er && col >= sc && col <= ec;
    if (row < sr || row > er) return 0;
    if (row == sr && row == er) return col >= sc && col <= ec;
    if (row == sr) return col >= sc;
    if (row == er) return col <= ec;
    return 1;
}

char *sel_get_text(Selection *sel) { return sel->clipboard; }

void sel_select_all(Selection *sel, TermState *ts)
{
    sel->start_row = 0; sel->start_col = 0;
    sel->end_row = ts->rows - 1; sel->end_col = ts->cols - 1;
    sel->mode = SEL_CHAR; sel->active = 0;
    sel_finish(sel, ts);
}

void sel_select_word(Selection *sel, TermState *ts, int row, int col)
{
    int left = col, right = col;
    while (left > 0) {
        TermCell *c = term_cell_at(ts, row, left - 1);
        if (!c || c->ch == ' ') break;
        left--;
    }
    while (right < ts->cols - 1) {
        TermCell *c = term_cell_at(ts, row, right + 1);
        if (!c || c->ch == ' ') break;
        right++;
    }
    sel->start_row = sel->end_row = row;
    sel->start_col = left; sel->end_col = right;
    sel->mode = SEL_WORD; sel->active = 0;
    sel_finish(sel, ts);
}

void sel_select_line(Selection *sel, TermState *ts, int row)
{
    sel->start_row = sel->end_row = row;
    sel->start_col = 0; sel->end_col = ts->cols - 1;
    sel->mode = SEL_LINE; sel->active = 0;
    sel_finish(sel, ts);
}
