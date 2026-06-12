/* puttyalt_grid.c - Generic grid layout (rows x cols with spans). */
#include <string.h>
#define GR_MAX 32
typedef struct {
    int row, col;
    int rowspan, colspan;
    int widget_id;
} gr_cell;
typedef struct {
    gr_cell cells[GR_MAX];
    int n;
    int rows, cols;
    int total_w, total_h;
    int gap;
} Grid;
void grid_init(Grid *g, int rows, int cols, int w, int h, int gap) {
    if (!g) return;
    memset(g, 0, sizeof(*g));
    g->rows = rows;
    g->cols = cols;
    g->total_w = w;
    g->total_h = h;
    g->gap = gap;
}
int grid_place(Grid *g, int widget_id, int row, int col, int rowspan, int colspan) {
    if (!g || g->n >= GR_MAX) return -1;
    if (row + rowspan > g->rows || col + colspan > g->cols) return -1;
    gr_cell *c = &g->cells[g->n];
    c->widget_id = widget_id;
    c->row = row;
    c->col = col;
    c->rowspan = rowspan > 0 ? rowspan : 1;
    c->colspan = colspan > 0 ? colspan : 1;
    return g->n++;
}
int grid_cell_rect(const Grid *g, int idx, int *x, int *y, int *w, int *h) {
    if (!g || idx < 0 || idx >= g->n || g->cols == 0 || g->rows == 0) return -1;
    const gr_cell *c = &g->cells[idx];
    int cw = (g->total_w - (g->cols + 1) * g->gap) / g->cols;
    int ch = (g->total_h - (g->rows + 1) * g->gap) / g->rows;
    if (x) *x = g->gap + c->col * (cw + g->gap);
    if (y) *y = g->gap + c->row * (ch + g->gap);
    if (w) *w = cw * c->colspan + (c->colspan - 1) * g->gap;
    if (h) *h = ch * c->rowspan + (c->rowspan - 1) * g->gap;
    return 0;
}
int grid_count(const Grid *g) {
    return g ? g->n : -1;
}
