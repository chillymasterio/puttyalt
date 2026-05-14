#ifndef PUTTYALT_SELECTION_H
#define PUTTYALT_SELECTION_H
#include "puttyalt_termemu.h"

typedef enum { SEL_NONE, SEL_CHAR, SEL_WORD, SEL_LINE, SEL_RECT } SelectMode;

typedef struct {
    int start_row, start_col;
    int end_row, end_col;
    SelectMode mode;
    int active;
    int anchor_row, anchor_col;
    char *clipboard;
    int clipboard_len;
} Selection;

void sel_init(Selection *sel);
void sel_destroy(Selection *sel);
void sel_start(Selection *sel, int row, int col, SelectMode mode);
void sel_update(Selection *sel, int row, int col);
void sel_finish(Selection *sel, TermState *ts);
void sel_clear(Selection *sel);
int  sel_is_selected(Selection *sel, int row, int col);
char *sel_get_text(Selection *sel);
void sel_select_all(Selection *sel, TermState *ts);
void sel_select_word(Selection *sel, TermState *ts, int row, int col);
void sel_select_line(Selection *sel, TermState *ts, int row);

#endif
