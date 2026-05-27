#include <string.h>
#include <stdio.h>
typedef struct { int x; int y; int w; int h; int maximized; int monitor; } WindowPos;
static WindowPos g_pos = {100, 100, 800, 600, 0, 0};
static WindowPos g_saved = {100, 100, 800, 600, 0, 0};
void windowpos_set(int x, int y, int w, int h) { g_pos.x=x; g_pos.y=y; g_pos.w=w; g_pos.h=h; }
void windowpos_save(void) { g_saved = g_pos; }
void windowpos_restore(WindowPos *out) { *out = g_saved; }
void windowpos_cascade(int instance, WindowPos *out) {
    out->x = 100 + instance * 30; out->y = 100 + instance * 30;
    out->w = 800; out->h = 600;
}
void windowpos_tile(int idx, int total, int screen_w, int screen_h, WindowPos *out) {
    int cols = total <= 2 ? total : (total <= 4 ? 2 : 3);
    int rows = (total + cols - 1) / cols;
    out->w = screen_w / cols; out->h = screen_h / rows;
    out->x = (idx % cols) * out->w; out->y = (idx / cols) * out->h;
}
