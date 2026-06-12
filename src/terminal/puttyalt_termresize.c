/* puttyalt_termresize.c - Terminal resize handler with content preservation. */
#include <string.h>
typedef struct { int rows, cols; int old_rows, old_cols; int cursor_row, cursor_col; int preserve_scroll; } TermResize;
void termresize_init(TermResize *t, int rows, int cols) {
    if(!t) return; memset(t,0,sizeof(*t)); t->rows=rows; t->cols=cols; t->preserve_scroll=1;
}
int termresize_apply(TermResize *t, int new_rows, int new_cols) {
    if(!t||new_rows<=0||new_cols<=0) return -1;
    t->old_rows=t->rows; t->old_cols=t->cols; t->rows=new_rows; t->cols=new_cols;
    /* clamp cursor */
    if (t->cursor_row>=new_rows) t->cursor_row=new_rows-1;
    if (t->cursor_col>=new_cols) t->cursor_col=new_cols-1;
    return 0;
}
int termresize_rows_delta(const TermResize *t) { return t?(t->rows-t->old_rows):0; }
int termresize_cols_delta(const TermResize *t) { return t?(t->cols-t->old_cols):0; }
int termresize_needs_reflow(const TermResize *t) { return (t&&t->cols!=t->old_cols)?1:0; }
void termresize_set_cursor(TermResize *t, int row, int col) {
    if(!t) return;
    t->cursor_row = row<0?0:(row>=t->rows?t->rows-1:row);
    t->cursor_col = col<0?0:(col>=t->cols?t->cols-1:col);
}
