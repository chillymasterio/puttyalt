#include <string.h>
#include "puttyalt_fontmgr.h"

void fontmgr_init(FontManager *fm)
{
    memset(fm, 0, sizeof(*fm));
    fm->current = -1;
    fm->fallback = -1;
}

int fontmgr_add(FontManager *fm, const char *name, int size)
{
    if (fm->count >= FONT_MAX_ENTRIES) return -1;
    FontEntry *f = &fm->fonts[fm->count];
    memset(f, 0, sizeof(*f));
    strncpy(f->name, name, FONT_NAME_LEN - 1);
    f->size = size;
    f->is_monospace = 1;
    if (fm->current < 0) fm->current = fm->count;
    return fm->count++;
}

int fontmgr_remove(FontManager *fm, int index)
{
    if (index < 0 || index >= fm->count) return -1;
    for (int i = index; i < fm->count - 1; i++)
        fm->fonts[i] = fm->fonts[i + 1];
    fm->count--;
    if (fm->current >= fm->count)
        fm->current = fm->count > 0 ? 0 : -1;
    return 0;
}

int fontmgr_set_current(FontManager *fm, int index)
{
    if (index < 0 || index >= fm->count) return -1;
    fm->current = index;
    return 0;
}

int fontmgr_set_size(FontManager *fm, int index, int size)
{
    if (index < 0 || index >= fm->count) return -1;
    if (size < 6 || size > 72) return -1;
    fm->fonts[index].size = size;
    return 0;
}

int fontmgr_find(const FontManager *fm, const char *name)
{
    for (int i = 0; i < fm->count; i++)
        if (strcmp(fm->fonts[i].name, name) == 0) return i;
    return -1;
}

int fontmgr_cycle_next(FontManager *fm)
{
    if (fm->count <= 1) return fm->current;
    fm->current = (fm->current + 1) % fm->count;
    return fm->current;
}
