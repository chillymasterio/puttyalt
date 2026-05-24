#ifndef PUTTYALT_TABPREVIEW_H
#define PUTTYALT_TABPREVIEW_H

typedef struct {
    int  enabled;
    int  delay_ms;
    int  preview_width;
    int  preview_height;
    int  show_session_info;
    int  show_last_output;
    int  last_output_lines;
    char cache_lines[8][256];
    int  cache_count;
    int  active_tab;
} TabPreview;

void tabpreview_init(TabPreview *tp);
void tabpreview_cache_output(TabPreview *tp, int tab, const char *line);
void tabpreview_clear_cache(TabPreview *tp, int tab);
int  tabpreview_should_show(TabPreview *tp, int hover_ms);
void tabpreview_set_config(TabPreview *tp, int width, int height, int delay);

#endif
