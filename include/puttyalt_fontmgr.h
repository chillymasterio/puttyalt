#ifndef PUTTYALT_FONTMGR_H
#define PUTTYALT_FONTMGR_H

#define FONT_MAX_ENTRIES  32
#define FONT_NAME_LEN     128

typedef struct FontEntry {
    char name[FONT_NAME_LEN];
    int size;
    int bold;
    int italic;
    int is_monospace;
    int line_spacing;
    int char_spacing;
} FontEntry;

typedef struct FontManager {
    FontEntry fonts[FONT_MAX_ENTRIES];
    int count;
    int current;
    int fallback;
} FontManager;

void fontmgr_init(FontManager *fm);
int  fontmgr_add(FontManager *fm, const char *name, int size);
int  fontmgr_remove(FontManager *fm, int index);
int  fontmgr_set_current(FontManager *fm, int index);
int  fontmgr_set_size(FontManager *fm, int index, int size);
int  fontmgr_find(const FontManager *fm, const char *name);
int  fontmgr_cycle_next(FontManager *fm);

#endif
