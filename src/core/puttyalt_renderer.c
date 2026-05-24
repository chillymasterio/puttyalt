#include "puttyalt_renderer.h"
#include <string.h>

void renderer_init(TermRenderer *r, int cols, int rows)
{
    memset(r, 0, sizeof(*r));
    r->cols = cols > RENDER_MAX_COLS ? RENDER_MAX_COLS : cols;
    r->rows = rows > RENDER_MAX_ROWS ? RENDER_MAX_ROWS : rows;
    r->cursor_visible = 1;
    r->default_fg = 0xD4DEE8;
    r->default_bg = 0x0D1520;
    r->scroll_bottom = r->rows - 1;
    r->dirty_from = 0;
    r->dirty_to = r->rows - 1;
    renderer_clear(r);
}

void renderer_resize(TermRenderer *r, int cols, int rows)
{
    if (cols > RENDER_MAX_COLS) cols = RENDER_MAX_COLS;
    if (rows > RENDER_MAX_ROWS) rows = RENDER_MAX_ROWS;
    r->cols = cols;
    r->rows = rows;
    r->scroll_bottom = rows - 1;
    if (r->cursor_x >= cols) r->cursor_x = cols - 1;
    if (r->cursor_y >= rows) r->cursor_y = rows - 1;
    renderer_mark_dirty(r, 0, rows - 1);
}

void renderer_clear(TermRenderer *r)
{
    for (int y = 0; y < r->rows; y++) {
        for (int x = 0; x < r->cols; x++) {
            r->cells[y][x].ch = ' ';
            r->cells[y][x].fg = r->default_fg;
            r->cells[y][x].bg = r->default_bg;
            r->cells[y][x].attr = 0;
        }
    }
    r->cursor_x = 0;
    r->cursor_y = 0;
    renderer_mark_dirty(r, 0, r->rows - 1);
}

void renderer_set_cell(TermRenderer *r, int x, int y, unsigned char ch,
                       unsigned int fg, unsigned int bg, unsigned char attr)
{
    if (x < 0 || x >= r->cols || y < 0 || y >= r->rows) return;
    RenderCell *c = &r->cells[y][x];
    c->ch = ch;
    c->fg = fg;
    c->bg = bg;
    c->attr = attr;
    renderer_mark_dirty(r, y, y);
}

void renderer_scroll_up(TermRenderer *r, int lines)
{
    if (lines <= 0) return;
    if (lines > r->scroll_bottom - r->scroll_top + 1)
        lines = r->scroll_bottom - r->scroll_top + 1;

    for (int y = r->scroll_top; y <= r->scroll_bottom - lines; y++)
        memcpy(r->cells[y], r->cells[y + lines],
               sizeof(RenderCell) * r->cols);

    for (int y = r->scroll_bottom - lines + 1; y <= r->scroll_bottom; y++) {
        for (int x = 0; x < r->cols; x++) {
            r->cells[y][x].ch = ' ';
            r->cells[y][x].fg = r->default_fg;
            r->cells[y][x].bg = r->default_bg;
            r->cells[y][x].attr = 0;
        }
    }
    renderer_mark_dirty(r, r->scroll_top, r->scroll_bottom);
}

void renderer_scroll_down(TermRenderer *r, int lines)
{
    if (lines <= 0) return;
    if (lines > r->scroll_bottom - r->scroll_top + 1)
        lines = r->scroll_bottom - r->scroll_top + 1;

    for (int y = r->scroll_bottom; y >= r->scroll_top + lines; y--)
        memcpy(r->cells[y], r->cells[y - lines],
               sizeof(RenderCell) * r->cols);

    for (int y = r->scroll_top; y < r->scroll_top + lines; y++) {
        for (int x = 0; x < r->cols; x++) {
            r->cells[y][x].ch = ' ';
            r->cells[y][x].fg = r->default_fg;
            r->cells[y][x].bg = r->default_bg;
            r->cells[y][x].attr = 0;
        }
    }
    renderer_mark_dirty(r, r->scroll_top, r->scroll_bottom);
}

void renderer_set_cursor(TermRenderer *r, int x, int y)
{
    if (x >= 0 && x < r->cols) r->cursor_x = x;
    if (y >= 0 && y < r->rows) r->cursor_y = y;
}

void renderer_mark_dirty(TermRenderer *r, int from, int to)
{
    if (from < r->dirty_from) r->dirty_from = from;
    if (to > r->dirty_to) r->dirty_to = to;
}

int renderer_is_dirty(const TermRenderer *r)
{
    return r->dirty_from <= r->dirty_to;
}

void renderer_clear_dirty(TermRenderer *r)
{
    r->dirty_from = r->rows;
    r->dirty_to = -1;
}
