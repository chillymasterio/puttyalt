#ifndef PUTTYALT_TERMEMU_H
#define PUTTYALT_TERMEMU_H
#include "puttyalt.h"
#include <stdint.h>

#define TERM_MAX_COLS  512
#define TERM_MAX_ROWS  256
#define TERM_TAB_STOP  8

typedef struct {
    uint32_t ch;
    uint8_t  fg;
    uint8_t  bg;
    uint8_t  attr; /* bold=1, underline=2, italic=4, blink=8, reverse=16, strike=32 */
} TermCell;

typedef struct {
    int rows, cols;
    int cursor_row, cursor_col;
    int cursor_visible;
    int cursor_style; /* 0=block, 1=underline, 2=bar */
    int scroll_top, scroll_bottom;
    int saved_row, saved_col;
    uint8_t default_fg, default_bg;
    uint8_t current_fg, current_bg;
    uint8_t current_attr;
    int charset; /* 0=UTF-8, 1=Latin-1, 2=CP437 */
    int autowrap;
    int insert_mode;
    int origin_mode;
    int bracketed_paste;
    int alt_screen;
    TermCell *cells;
    TermCell *alt_cells;
    int dirty;
} TermState;

int  term_init(TermState *ts, int rows, int cols);
void term_destroy(TermState *ts);
void term_resize(TermState *ts, int rows, int cols);
void term_reset(TermState *ts);
void term_write(TermState *ts, const uint8_t *data, int len);
void term_set_cursor(TermState *ts, int row, int col);
void term_scroll_up(TermState *ts, int n);
void term_scroll_down(TermState *ts, int n);
void term_erase_line(TermState *ts, int mode);
void term_erase_screen(TermState *ts, int mode);
void term_insert_chars(TermState *ts, int n);
void term_delete_chars(TermState *ts, int n);
TermCell *term_cell_at(TermState *ts, int row, int col);
void term_save_cursor(TermState *ts);
void term_restore_cursor(TermState *ts);
void term_switch_screen(TermState *ts, int alt);

#endif
