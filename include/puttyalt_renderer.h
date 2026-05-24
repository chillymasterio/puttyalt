#ifndef PUTTYALT_RENDERER_H
#define PUTTYALT_RENDERER_H

#define RENDER_MAX_COLS  512
#define RENDER_MAX_ROWS  256

typedef struct {
    unsigned int fg;
    unsigned int bg;
    unsigned char ch;
    unsigned char attr;  /* bold, italic, underline, blink flags */
} RenderCell;

typedef struct {
    RenderCell cells[RENDER_MAX_ROWS][RENDER_MAX_COLS];
    int cols, rows;
    int cursor_x, cursor_y;
    int cursor_visible;
    int scroll_top, scroll_bottom;
    int dirty_from, dirty_to;  /* dirty row range */
    unsigned int default_fg;
    unsigned int default_bg;
    int line_spacing;
} TermRenderer;

void renderer_init(TermRenderer *r, int cols, int rows);
void renderer_resize(TermRenderer *r, int cols, int rows);
void renderer_clear(TermRenderer *r);
void renderer_set_cell(TermRenderer *r, int x, int y, unsigned char ch,
                       unsigned int fg, unsigned int bg, unsigned char attr);
void renderer_scroll_up(TermRenderer *r, int lines);
void renderer_scroll_down(TermRenderer *r, int lines);
void renderer_set_cursor(TermRenderer *r, int x, int y);
void renderer_mark_dirty(TermRenderer *r, int row_from, int row_to);
int  renderer_is_dirty(const TermRenderer *r);
void renderer_clear_dirty(TermRenderer *r);

#endif
