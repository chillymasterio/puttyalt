#include "puttyalt_tabpreview.h"
#include <string.h>

void tabpreview_init(TabPreview *tp)
{
    memset(tp, 0, sizeof(*tp));
    tp->enabled = 1;
    tp->delay_ms = 500;
    tp->preview_width = 320;
    tp->preview_height = 200;
    tp->show_session_info = 1;
    tp->show_last_output = 1;
    tp->last_output_lines = 8;
}

void tabpreview_cache_output(TabPreview *tp, int tab, const char *line)
{
    (void)tab;
    if (tp->cache_count < 8) {
        snprintf(tp->cache_lines[tp->cache_count++], 256, "%s", line);
    } else {
        memmove(tp->cache_lines[0], tp->cache_lines[1], 7 * 256);
        snprintf(tp->cache_lines[7], 256, "%s", line);
    }
}

void tabpreview_clear_cache(TabPreview *tp, int tab) { (void)tab; tp->cache_count = 0; }
int tabpreview_should_show(TabPreview *tp, int hover_ms) { return tp->enabled && hover_ms >= tp->delay_ms; }
void tabpreview_set_config(TabPreview *tp, int w, int h, int delay) { tp->preview_width = w; tp->preview_height = h; tp->delay_ms = delay; }
