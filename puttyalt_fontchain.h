#ifndef PUTTYALT_FONTCHAIN_H
#define PUTTYALT_FONTCHAIN_H

#define FONT_CHAIN_MAX 8

typedef struct {
    char names[FONT_CHAIN_MAX][64];
    int count;
    int size;
    int bold;
    int italic;
    int antialias;
    int hinting;
    float line_spacing;
    float letter_spacing;
} FontChain;

void fontchain_init(FontChain *fc);
int  fontchain_add(FontChain *fc, const char *name);
void fontchain_remove(FontChain *fc, int index);
void fontchain_set_size(FontChain *fc, int size);
const char *fontchain_get(FontChain *fc, int index);
int  fontchain_load(FontChain *fc, const char *path);
int  fontchain_save(const FontChain *fc, const char *path);

#endif
