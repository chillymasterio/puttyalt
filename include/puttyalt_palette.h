#ifndef PUTTYALT_PALETTE_H
#define PUTTYALT_PALETTE_H

#define PAL_SIZE 256    /* 16 ANSI + 216 cube + 24 grayscale */

typedef struct Palette {
    unsigned int colors[PAL_SIZE]; /* RGB packed */
    int modified;
    char name[64];
} Palette;

void palette_init_default(Palette *pal);
void palette_set_color(Palette *pal, int index, unsigned int rgb);
unsigned int palette_get_color(const Palette *pal, int index);
void palette_init_ansi(Palette *pal);
int  palette_export(const Palette *pal, const char *path);
int  palette_import(Palette *pal, const char *path);

#endif
