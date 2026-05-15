#ifndef PUTTYALT_TOOLTIP_H
#define PUTTYALT_TOOLTIP_H

typedef struct {
    char text[512];
    int x, y;
    int width, height;
    int visible;
    int delay_ms;
    int duration_ms;
    unsigned long show_time;
    int owner_id;
} Tooltip;

void tooltip_init(Tooltip *tt);
void tooltip_show(Tooltip *tt, const char *text, int x, int y);
void tooltip_hide(Tooltip *tt);
void tooltip_update(Tooltip *tt);
int  tooltip_should_show(Tooltip *tt);

#endif
