#include "puttyalt_termemu.h"
#include <stdlib.h>
#include <string.h>

int term_init(TermState *ts, int rows, int cols)
{
    memset(ts, 0, sizeof(*ts));
    ts->rows = rows > 0 ? rows : 24;
    ts->cols = cols > 0 ? cols : 80;
    ts->scroll_top = 0;
    ts->scroll_bottom = ts->rows - 1;
    ts->default_fg = 7;
    ts->default_bg = 0;
    ts->current_fg = 7;
    ts->current_bg = 0;
    ts->cursor_visible = 1;
    ts->cursor_style = 0;
    ts->autowrap = 1;
    ts->cells = calloc(ts->rows * ts->cols, sizeof(TermCell));
    ts->alt_cells = calloc(ts->rows * ts->cols, sizeof(TermCell));
    if (!ts->cells || !ts->alt_cells) return -1;
    for (int i = 0; i < ts->rows * ts->cols; i++) {
        ts->cells[i].ch = ' ';
        ts->cells[i].fg = ts->default_fg;
        ts->cells[i].bg = ts->default_bg;
        ts->alt_cells[i] = ts->cells[i];
    }
    return 0;
}

void term_destroy(TermState *ts)
{
    free(ts->cells); free(ts->alt_cells);
    ts->cells = ts->alt_cells = NULL;
}

void term_resize(TermState *ts, int rows, int cols)
{
    if (rows <= 0 || cols <= 0 || rows > TERM_MAX_ROWS || cols > TERM_MAX_COLS) return;
    TermCell *nc = calloc(rows * cols, sizeof(TermCell));
    if (!nc) return;
    for (int i = 0; i < rows * cols; i++) { nc[i].ch = ' '; nc[i].fg = ts->default_fg; nc[i].bg = ts->default_bg; }
    int cr = rows < ts->rows ? rows : ts->rows;
    int cc = cols < ts->cols ? cols : ts->cols;
    for (int r = 0; r < cr; r++)
        memcpy(&nc[r * cols], &ts->cells[r * ts->cols], cc * sizeof(TermCell));
    free(ts->cells); ts->cells = nc;
    ts->rows = rows; ts->cols = cols;
    ts->scroll_bottom = rows - 1;
    if (ts->cursor_row >= rows) ts->cursor_row = rows - 1;
    if (ts->cursor_col >= cols) ts->cursor_col = cols - 1;
    ts->dirty = 1;
}

void term_reset(TermState *ts)
{
    ts->cursor_row = ts->cursor_col = 0;
    ts->current_fg = ts->default_fg;
    ts->current_bg = ts->default_bg;
    ts->current_attr = 0;
    ts->scroll_top = 0;
    ts->scroll_bottom = ts->rows - 1;
    ts->insert_mode = 0;
    ts->origin_mode = 0;
    ts->autowrap = 1;
    ts->cursor_visible = 1;
    term_erase_screen(ts, 2);
}

TermCell *term_cell_at(TermState *ts, int row, int col)
{
    if (row < 0 || row >= ts->rows || col < 0 || col >= ts->cols) return NULL;
    return &ts->cells[row * ts->cols + col];
}

void term_set_cursor(TermState *ts, int row, int col)
{
    if (row < 0) row = 0; if (row >= ts->rows) row = ts->rows - 1;
    if (col < 0) col = 0; if (col >= ts->cols) col = ts->cols - 1;
    ts->cursor_row = row; ts->cursor_col = col;
}

void term_scroll_up(TermState *ts, int n)
{
    if (n <= 0) return;
    int top = ts->scroll_top, bot = ts->scroll_bottom;
    int range = bot - top + 1;
    if (n > range) n = range;
    memmove(&ts->cells[top * ts->cols], &ts->cells[(top + n) * ts->cols],
            (range - n) * ts->cols * sizeof(TermCell));
    for (int r = bot - n + 1; r <= bot; r++)
        for (int c = 0; c < ts->cols; c++) {
            ts->cells[r * ts->cols + c].ch = ' ';
            ts->cells[r * ts->cols + c].fg = ts->default_fg;
            ts->cells[r * ts->cols + c].bg = ts->default_bg;
            ts->cells[r * ts->cols + c].attr = 0;
        }
    ts->dirty = 1;
}

void term_scroll_down(TermState *ts, int n)
{
    if (n <= 0) return;
    int top = ts->scroll_top, bot = ts->scroll_bottom;
    int range = bot - top + 1;
    if (n > range) n = range;
    memmove(&ts->cells[(top + n) * ts->cols], &ts->cells[top * ts->cols],
            (range - n) * ts->cols * sizeof(TermCell));
    for (int r = top; r < top + n; r++)
        for (int c = 0; c < ts->cols; c++) {
            ts->cells[r * ts->cols + c].ch = ' ';
            ts->cells[r * ts->cols + c].fg = ts->default_fg;
            ts->cells[r * ts->cols + c].bg = ts->default_bg;
            ts->cells[r * ts->cols + c].attr = 0;
        }
    ts->dirty = 1;
}

void term_erase_line(TermState *ts, int mode)
{
    int start = 0, end = ts->cols;
    if (mode == 0) start = ts->cursor_col;
    else if (mode == 1) end = ts->cursor_col + 1;
    for (int c = start; c < end; c++) {
        TermCell *cell = term_cell_at(ts, ts->cursor_row, c);
        if (cell) { cell->ch = ' '; cell->fg = ts->current_fg; cell->bg = ts->current_bg; cell->attr = 0; }
    }
    ts->dirty = 1;
}

void term_erase_screen(TermState *ts, int mode)
{
    int sr = 0, er = ts->rows;
    if (mode == 0) { sr = ts->cursor_row; }
    else if (mode == 1) { er = ts->cursor_row + 1; }
    for (int r = sr; r < er; r++)
        for (int c = 0; c < ts->cols; c++) {
            ts->cells[r * ts->cols + c].ch = ' ';
            ts->cells[r * ts->cols + c].fg = ts->default_fg;
            ts->cells[r * ts->cols + c].bg = ts->default_bg;
            ts->cells[r * ts->cols + c].attr = 0;
        }
    ts->dirty = 1;
}

void term_insert_chars(TermState *ts, int n)
{
    if (n <= 0) return;
    int row = ts->cursor_row, col = ts->cursor_col;
    int shift = ts->cols - col - n;
    if (shift > 0)
        memmove(&ts->cells[row * ts->cols + col + n], &ts->cells[row * ts->cols + col], shift * sizeof(TermCell));
    for (int c = col; c < col + n && c < ts->cols; c++) {
        ts->cells[row * ts->cols + c].ch = ' ';
        ts->cells[row * ts->cols + c].fg = ts->current_fg;
        ts->cells[row * ts->cols + c].bg = ts->current_bg;
        ts->cells[row * ts->cols + c].attr = 0;
    }
    ts->dirty = 1;
}

void term_delete_chars(TermState *ts, int n)
{
    if (n <= 0) return;
    int row = ts->cursor_row, col = ts->cursor_col;
    int shift = ts->cols - col - n;
    if (shift > 0)
        memmove(&ts->cells[row * ts->cols + col], &ts->cells[row * ts->cols + col + n], shift * sizeof(TermCell));
    for (int c = ts->cols - n; c < ts->cols; c++) {
        ts->cells[row * ts->cols + c].ch = ' ';
        ts->cells[row * ts->cols + c].fg = ts->current_fg;
        ts->cells[row * ts->cols + c].bg = ts->current_bg;
        ts->cells[row * ts->cols + c].attr = 0;
    }
    ts->dirty = 1;
}

void term_save_cursor(TermState *ts) { ts->saved_row = ts->cursor_row; ts->saved_col = ts->cursor_col; }
void term_restore_cursor(TermState *ts) { term_set_cursor(ts, ts->saved_row, ts->saved_col); }

void term_switch_screen(TermState *ts, int alt)
{
    if ((alt && ts->alt_screen) || (!alt && !ts->alt_screen)) return;
    TermCell *tmp = ts->cells;
    ts->cells = ts->alt_cells;
    ts->alt_cells = tmp;
    ts->alt_screen = alt;
    ts->dirty = 1;
}

void term_write(TermState *ts, const uint8_t *data, int len)
{
    for (int i = 0; i < len; i++) {
        uint8_t ch = data[i];
        if (ch == '\n') {
            if (ts->cursor_row >= ts->scroll_bottom) term_scroll_up(ts, 1);
            else ts->cursor_row++;
        } else if (ch == '\r') {
            ts->cursor_col = 0;
        } else if (ch == '\b') {
            if (ts->cursor_col > 0) ts->cursor_col--;
        } else if (ch == '\t') {
            ts->cursor_col = ((ts->cursor_col / TERM_TAB_STOP) + 1) * TERM_TAB_STOP;
            if (ts->cursor_col >= ts->cols) ts->cursor_col = ts->cols - 1;
        } else if (ch == '\a') {
            /* bell — handled by GUI layer */
        } else if (ch >= 0x20) {
            TermCell *cell = term_cell_at(ts, ts->cursor_row, ts->cursor_col);
            if (cell) {
                cell->ch = ch; cell->fg = ts->current_fg;
                cell->bg = ts->current_bg; cell->attr = ts->current_attr;
            }
            ts->cursor_col++;
            if (ts->cursor_col >= ts->cols) {
                if (ts->autowrap) {
                    ts->cursor_col = 0;
                    if (ts->cursor_row >= ts->scroll_bottom) term_scroll_up(ts, 1);
                    else ts->cursor_row++;
                } else {
                    ts->cursor_col = ts->cols - 1;
                }
            }
        }
    }
    ts->dirty = 1;
}
