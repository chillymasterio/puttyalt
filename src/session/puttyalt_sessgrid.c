/* puttyalt_sessgrid.c - Session grid view (tile layout calculation). */
#include <string.h>
typedef struct { int cols, rows; int cell_w, cell_h; int gap; int total_w, total_h; } SessGrid;
void sessgrid_init(SessGrid *g, int total_w, int total_h, int gap) {
    if(!g) return;
    memset(g,0,sizeof(*g)); g->total_w=total_w; g->total_h=total_h; g->gap=gap;
}
int sessgrid_layout(SessGrid *g, int n_items) {
    if(!g||n_items<=0) return -1;
    /* find cols/rows closest to square */
    int cols=1; while (cols*cols<n_items) cols++;
    int rows=(n_items+cols-1)/cols;
    g->cols=cols; g->rows=rows;
    g->cell_w=(g->total_w-(cols+1)*g->gap)/cols;
    g->cell_h=(g->total_h-(rows+1)*g->gap)/rows;
    return 0;
}
int sessgrid_cell_rect(const SessGrid *g, int index, int *x, int *y, int *w, int *h) {
    if(!g||g->cols==0) return -1;
    int col=index%g->cols, row=index/g->cols;
    if (x) *x=g->gap+col*(g->cell_w+g->gap);
    if (y) *y=g->gap+row*(g->cell_h+g->gap);
    if (w) *w=g->cell_w;
    if (h) *h=g->cell_h;
    return 0;
}
int sessgrid_hit(const SessGrid *g, int px, int py) {
    if(!g||g->cell_w<=0||g->cell_h<=0) return -1;
    int col=(px-g->gap)/(g->cell_w+g->gap), row=(py-g->gap)/(g->cell_h+g->gap);
    if (col<0||col>=g->cols||row<0||row>=g->rows) return -1;
    return row*g->cols+col;
}
