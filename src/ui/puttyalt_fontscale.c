#include <string.h>
#include <stdio.h>
typedef struct { int base_size; int current_size; int min_size; int max_size; int step; } FontScale;
static FontScale g_scale = {11, 11, 6, 36, 1};
void fontscale_init(int base) { g_scale.base_size = base; g_scale.current_size = base; }
int fontscale_zoom_in(void) {
    if (g_scale.current_size + g_scale.step <= g_scale.max_size) g_scale.current_size += g_scale.step;
    return g_scale.current_size;
}
int fontscale_zoom_out(void) {
    if (g_scale.current_size - g_scale.step >= g_scale.min_size) g_scale.current_size -= g_scale.step;
    return g_scale.current_size;
}
int fontscale_reset(void) { g_scale.current_size = g_scale.base_size; return g_scale.current_size; }
int fontscale_get(void) { return g_scale.current_size; }
int fontscale_get_pct(void) { return g_scale.current_size * 100 / g_scale.base_size; }
