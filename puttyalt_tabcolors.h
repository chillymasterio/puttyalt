#ifndef PUTTYALT_TABCOLORS_H
#define PUTTYALT_TABCOLORS_H

#define TAB_COLOR_NONE   0
#define TAB_COLOR_RED    1
#define TAB_COLOR_GREEN  2
#define TAB_COLOR_BLUE   3
#define TAB_COLOR_YELLOW 4
#define TAB_COLOR_PURPLE 5
#define TAB_COLOR_ORANGE 6
#define TAB_COLOR_CYAN   7
#define TAB_COLOR_MAX    8

typedef struct {
    char label[TAB_COLOR_MAX][16];
    unsigned int rgb[TAB_COLOR_MAX];
    int auto_assign; /* auto-assign colors to new tabs */
    int show_dot;    /* show color dot on tab */
    int show_underline; /* show colored underline */
} TabColorConfig;

void tabcolor_init(TabColorConfig *tc);
const char *tabcolor_label(TabColorConfig *tc, int color);
unsigned int tabcolor_rgb(TabColorConfig *tc, int color);
int tabcolor_next_auto(TabColorConfig *tc, int current);

#endif
