#include "puttyalt_tabcolors.h"
#include <string.h>

void tabcolor_init(TabColorConfig *tc)
{
    memset(tc, 0, sizeof(*tc));
    snprintf(tc->label[0], 16, "None");
    snprintf(tc->label[1], 16, "Production");
    snprintf(tc->label[2], 16, "Staging");
    snprintf(tc->label[3], 16, "Development");
    snprintf(tc->label[4], 16, "Warning");
    snprintf(tc->label[5], 16, "Database");
    snprintf(tc->label[6], 16, "Monitoring");
    snprintf(tc->label[7], 16, "Other");
    tc->rgb[0] = 0x000000;
    tc->rgb[1] = 0xE74C3C; /* red */
    tc->rgb[2] = 0x2ECC71; /* green */
    tc->rgb[3] = 0x3498DB; /* blue */
    tc->rgb[4] = 0xF39C12; /* yellow */
    tc->rgb[5] = 0x9B59B6; /* purple */
    tc->rgb[6] = 0xE67E22; /* orange */
    tc->rgb[7] = 0x1ABC9C; /* cyan */
    tc->show_dot = 1;
    tc->show_underline = 1;
}

const char *tabcolor_label(TabColorConfig *tc, int color)
{
    return (color >= 0 && color < TAB_COLOR_MAX) ? tc->label[color] : "Unknown";
}

unsigned int tabcolor_rgb(TabColorConfig *tc, int color)
{
    return (color >= 0 && color < TAB_COLOR_MAX) ? tc->rgb[color] : 0;
}

int tabcolor_next_auto(TabColorConfig *tc, int current)
{
    (void)tc;
    return (current + 1) % TAB_COLOR_MAX;
}
