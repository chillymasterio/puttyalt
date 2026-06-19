/* puttyalt_tablefmt.c - Format aligned text tables.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
#define TBF_COLS 8
typedef struct { int widths[TBF_COLS]; int ncols; } TableFmt;
void tbf_init(TableFmt *t, int ncols) {
    if (!t) return;
    t->ncols = (ncols > 0 && ncols <= TBF_COLS) ? ncols : TBF_COLS;
    for (int i = 0; i < TBF_COLS; i++) t->widths[i] = 0;
}
/* Update column widths from a row of cell strings. */
void tbf_measure(TableFmt *t, const char *const *cells) {
    if (!t || !cells) return;
    for (int i = 0; i < t->ncols; i++) {
        int l = (int)strlen(cells[i]);
        if (l > t->widths[i]) t->widths[i] = l;
    }
}
/* Render one row padded to measured widths. */
int tbf_row(const TableFmt *t, const char *const *cells, char *out, int outlen) {
    if (!t || !cells || !out) return -1;
    int o = 0;
    for (int i = 0; i < t->ncols && o < outlen - 1; i++) {
        int n = snprintf(out + o, outlen - o, "%-*s%s", t->widths[i], cells[i],
                         i < t->ncols - 1 ? "  " : "");
        if (n < 0) return -1;
        o += n;
    }
    return o;
}
