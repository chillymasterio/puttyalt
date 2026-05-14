#ifndef PUTTYALT_COLORMGR_H
#define PUTTYALT_COLORMGR_H
#include <stdint.h>

typedef struct { uint8_t r, g, b; } RGB;

typedef struct {
    char name[64];
    RGB palette[256];
    RGB fg, bg, cursor, selection;
    int bold_is_bright;
} ColorScheme;

void   colormgr_init_default(ColorScheme *cs);
void   colormgr_set_ansi16(ColorScheme *cs);
void   colormgr_set_xterm256(ColorScheme *cs);
RGB    colormgr_get(ColorScheme *cs, int index);
void   colormgr_set(ColorScheme *cs, int index, RGB color);
int    colormgr_load(ColorScheme *cs, const char *path);
int    colormgr_save(const ColorScheme *cs, const char *path);
RGB    colormgr_blend(RGB a, RGB b, float t);
uint32_t colormgr_to_win32(RGB c);

#endif
