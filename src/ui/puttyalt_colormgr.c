#include "puttyalt_colormgr.h"
#include <string.h>
#include <stdio.h>

static const RGB ansi16[] = {
    {0,0,0}, {170,0,0}, {0,170,0}, {170,85,0},
    {0,0,170}, {170,0,170}, {0,170,170}, {170,170,170},
    {85,85,85}, {255,85,85}, {85,255,85}, {255,255,85},
    {85,85,255}, {255,85,255}, {85,255,255}, {255,255,255},
};

void colormgr_init_default(ColorScheme *cs)
{
    memset(cs, 0, sizeof(*cs));
    snprintf(cs->name, sizeof(cs->name), "Warm Blue");
    colormgr_set_ansi16(cs);
    colormgr_set_xterm256(cs);
    cs->fg = (RGB){212, 222, 232};
    cs->bg = (RGB){13, 21, 32};
    cs->cursor = (RGB){74, 158, 224};
    cs->selection = (RGB){46, 92, 138};
    cs->bold_is_bright = 1;
}

void colormgr_set_ansi16(ColorScheme *cs)
{
    for (int i = 0; i < 16; i++) cs->palette[i] = ansi16[i];
}

void colormgr_set_xterm256(ColorScheme *cs)
{
    colormgr_set_ansi16(cs);
    /* 216-color cube (indices 16-231) */
    for (int r = 0; r < 6; r++)
        for (int g = 0; g < 6; g++)
            for (int b = 0; b < 6; b++) {
                int idx = 16 + r * 36 + g * 6 + b;
                cs->palette[idx].r = r ? (uint8_t)(r * 40 + 55) : 0;
                cs->palette[idx].g = g ? (uint8_t)(g * 40 + 55) : 0;
                cs->palette[idx].b = b ? (uint8_t)(b * 40 + 55) : 0;
            }
    /* Grayscale (indices 232-255) */
    for (int i = 0; i < 24; i++) {
        uint8_t v = (uint8_t)(i * 10 + 8);
        cs->palette[232 + i] = (RGB){v, v, v};
    }
}

RGB colormgr_get(ColorScheme *cs, int index)
{
    if (index < 0 || index > 255) return cs->fg;
    return cs->palette[index];
}

void colormgr_set(ColorScheme *cs, int index, RGB color)
{
    if (index >= 0 && index <= 255) cs->palette[index] = color;
}

int colormgr_load(ColorScheme *cs, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        int idx, r, g, b;
        if (sscanf(line, "color%d=%d,%d,%d", &idx, &r, &g, &b) == 4 && idx >= 0 && idx <= 255)
            cs->palette[idx] = (RGB){(uint8_t)r, (uint8_t)g, (uint8_t)b};
        else if (sscanf(line, "fg=%d,%d,%d", &r, &g, &b) == 3) cs->fg = (RGB){(uint8_t)r, (uint8_t)g, (uint8_t)b};
        else if (sscanf(line, "bg=%d,%d,%d", &r, &g, &b) == 3) cs->bg = (RGB){(uint8_t)r, (uint8_t)g, (uint8_t)b};
        else if (sscanf(line, "cursor=%d,%d,%d", &r, &g, &b) == 3) cs->cursor = (RGB){(uint8_t)r, (uint8_t)g, (uint8_t)b};
        else if (sscanf(line, "name=%63[^\n]", cs->name) == 1) { /* parsed */ }
    }
    fclose(f);
    return 0;
}

int colormgr_save(const ColorScheme *cs, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "name=%s\n", cs->name);
    fprintf(f, "fg=%d,%d,%d\n", cs->fg.r, cs->fg.g, cs->fg.b);
    fprintf(f, "bg=%d,%d,%d\n", cs->bg.r, cs->bg.g, cs->bg.b);
    fprintf(f, "cursor=%d,%d,%d\n", cs->cursor.r, cs->cursor.g, cs->cursor.b);
    fprintf(f, "selection=%d,%d,%d\n", cs->selection.r, cs->selection.g, cs->selection.b);
    for (int i = 0; i < 256; i++)
        fprintf(f, "color%d=%d,%d,%d\n", i, cs->palette[i].r, cs->palette[i].g, cs->palette[i].b);
    fclose(f);
    return 0;
}

RGB colormgr_blend(RGB a, RGB b, float t)
{
    return (RGB){
        (uint8_t)(a.r + (b.r - a.r) * t),
        (uint8_t)(a.g + (b.g - a.g) * t),
        (uint8_t)(a.b + (b.b - a.b) * t),
    };
}

uint32_t colormgr_to_win32(RGB c) { return (uint32_t)c.r | ((uint32_t)c.g << 8) | ((uint32_t)c.b << 16); }
