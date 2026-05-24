#ifndef PUTTYALT_TOOLTIP_H
#define PUTTYALT_TOOLTIP_H

#define TOOLTIP_MAX_TEXT 256
#define TOOLTIP_DELAY_MS 500
#define TOOLTIP_FADE_MS  200

typedef struct {
    char text[TOOLTIP_MAX_TEXT];
    int x, y;
    int visible;
    int width, height;
    long show_time;
    long hover_start;
    float opacity;
    int active;
} Tooltip;

void tooltip_init(Tooltip *tt);
void tooltip_set(Tooltip *tt, const char *text, int x, int y);
void tooltip_hide(Tooltip *tt);
void tooltip_update(Tooltip *tt, long now_ms);
int  tooltip_should_show(const Tooltip *tt);

#endif
