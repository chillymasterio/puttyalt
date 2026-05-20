#ifndef PUTTYALT_TABOVERFLOW_H
#define PUTTYALT_TABOVERFLOW_H

#define TAB_OVERFLOW_NONE    0
#define TAB_OVERFLOW_SCROLL  1
#define TAB_OVERFLOW_COMPACT 2
#define TAB_OVERFLOW_MENU    3

typedef struct {
    int mode;
    int scroll_offset;
    int visible_tabs;
    int total_tabs;
    int min_tab_width;
    int compact_threshold;
    int show_arrows;
    int arrow_hover_left;
    int arrow_hover_right;
} TabOverflow;

void tab_overflow_init(TabOverflow *to);
void tab_overflow_update(TabOverflow *to, int total, int container_width, int tab_width);
int  tab_overflow_scroll_left(TabOverflow *to);
int  tab_overflow_scroll_right(TabOverflow *to);
int  tab_overflow_get_offset(const TabOverflow *to);
int  tab_overflow_needs_arrows(const TabOverflow *to);

#endif
