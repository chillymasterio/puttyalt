#include <string.h>
#include <stdio.h>
#include "puttyalt_palette.h"

static const unsigned int ansi_defaults[16] = {
    0x000000, 0xBB0000, 0x00BB00, 0xBBBB00,
    0x0000BB, 0xBB00BB, 0x00BBBB, 0xBBBBBB,
    0x555555, 0xFF5555, 0x55FF55, 0xFFFF55,
    0x5555FF, 0xFF55FF, 0x55FFFF, 0xFFFFFF
};

void palette_init_ansi(Palette *pal)
{
    memset(pal, 0, sizeof(*pal));
    for (int i = 0; i < 16; i++)
        pal->colors[i] = ansi_defaults[i];
}

void palette_init_default(Palette *pal)
{
    palette_init_ansi(pal);
    /* 216 color cube (indices 16-231) */
    int idx = 16;
    for (int r = 0; r < 6; r++)
        for (int g = 0; g < 6; g++)
            for (int b = 0; b < 6; b++)
                pal->colors[idx++] = ((r * 51) << 16) | ((g * 51) << 8) | (b * 51);
    /* 24 grayscale (indices 232-255) */
    for (int i = 0; i < 24; i++) {
        int v = 8 + i * 10;
        pal->colors[idx++] = (v << 16) | (v << 8) | v;
    }
    strncpy(pal->name, "Default", sizeof(pal->name) - 1);
}

void palette_set_color(Palette *pal, int index, unsigned int rgb)
{
    if (index >= 0 && index < PAL_SIZE) {
        pal->colors[index] = rgb;
        pal->modified = 1;
    }
}

unsigned int palette_get_color(const Palette *pal, int index)
{
    if (index >= 0 && index < PAL_SIZE) return pal->colors[index];
    return 0;
}

int palette_export(const Palette *pal, const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) return -1;
    fprintf(fp, "# PuttyAlt Palette: %s\n", pal->name);
    for (int i = 0; i < PAL_SIZE; i++)
        fprintf(fp, "%d=#%06X\n", i, pal->colors[i]);
    fclose(fp);
    return 0;
}

int palette_import(Palette *pal, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#') continue;
        int idx; unsigned int color;
        if (sscanf(line, "%d=#%x", &idx, &color) == 2)
            palette_set_color(pal, idx, color);
    }
    fclose(fp);
    return 0;
}
