/* puttyalt_scrollregion2.c - Scroll region v2 with left/right margins (DECSLRM). */
#include <string.h>
typedef struct {
    int top, bottom, left, right;
    int rows, cols;
    int margins_enabled;
} ScrollRegion2;
void scrollregion2_init(ScrollRegion2 *s, int rows, int cols) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
    s->rows = rows;
    s->cols = cols;
    s->bottom = rows - 1;
    s->right = cols - 1;
}
int scrollregion2_set_vertical(ScrollRegion2 *s, int top, int bottom) {
    if (!s) return -1;
    if (top < 0) top = 0;
    if (bottom >= s->rows) bottom = s->rows - 1;
    if (top >= bottom) return -1;
    s->top = top;
    s->bottom = bottom;
    return 0;
}
int scrollregion2_set_horizontal(ScrollRegion2 *s, int left, int right) {
    if (!s) return -1;
    if (left < 0) left = 0;
    if (right >= s->cols) right = s->cols - 1;
    if (left >= right) return -1;
    s->left = left;
    s->right = right;
    s->margins_enabled = 1;
    return 0;
}
int scrollregion2_contains(const ScrollRegion2 *s, int row, int col) {
    if (!s) return 0;
    if (row < s->top || row > s->bottom) return 0;
    if (s->margins_enabled && (col < s->left || col > s->right)) return 0;
    return 1;
}
void scrollregion2_reset(ScrollRegion2 *s) {
    if (!s) return;
    s->top = 0;
    s->bottom = s->rows - 1;
    s->left = 0;
    s->right = s->cols - 1;
    s->margins_enabled = 0;
}
