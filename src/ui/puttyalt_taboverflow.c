#include "puttyalt_taboverflow.h"

void tab_overflow_init(TabOverflow *to)
{
    to->mode = TAB_OVERFLOW_SCROLL;
    to->scroll_offset = 0;
    to->visible_tabs = 0;
    to->total_tabs = 0;
    to->min_tab_width = 80;
    to->compact_threshold = 12;
    to->show_arrows = 0;
    to->arrow_hover_left = 0;
    to->arrow_hover_right = 0;
}

void tab_overflow_update(TabOverflow *to, int total, int container_width, int tab_width)
{
    to->total_tabs = total;
    to->visible_tabs = container_width / (tab_width + 4);
    if (to->visible_tabs < 1) to->visible_tabs = 1;
    if (to->visible_tabs >= total) {
        to->show_arrows = 0;
        to->scroll_offset = 0;
    } else {
        to->show_arrows = 1;
        if (to->scroll_offset > total - to->visible_tabs)
            to->scroll_offset = total - to->visible_tabs;
    }

    if (total > to->compact_threshold)
        to->mode = TAB_OVERFLOW_COMPACT;
    else
        to->mode = TAB_OVERFLOW_SCROLL;
}

int tab_overflow_scroll_left(TabOverflow *to)
{
    if (to->scroll_offset > 0) {
        to->scroll_offset--;
        return 1;
    }
    return 0;
}

int tab_overflow_scroll_right(TabOverflow *to)
{
    if (to->scroll_offset < to->total_tabs - to->visible_tabs) {
        to->scroll_offset++;
        return 1;
    }
    return 0;
}

int tab_overflow_get_offset(const TabOverflow *to)
{
    return to->scroll_offset;
}

int tab_overflow_needs_arrows(const TabOverflow *to)
{
    return to->show_arrows;
}
