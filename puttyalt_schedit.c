#include "puttyalt_schedit.h"
#include <string.h>

void schedit_init(SchemeEditor *se, const ColorScheme *base)
{
    memset(se, 0, sizeof(*se));
    if (base) se->scheme = *base;
    else colormgr_init_default(&se->scheme);
}

void schedit_set_color(SchemeEditor *se, int index, RGB color)
{
    colormgr_set(&se->scheme, index, color);
    se->modified = 1;
}

void schedit_reset_to_default(SchemeEditor *se)
{
    colormgr_init_default(&se->scheme);
    se->modified = 1;
}

int schedit_export(SchemeEditor *se, const char *path)
{
    snprintf(se->export_path, sizeof(se->export_path), "%s", path);
    return colormgr_save(&se->scheme, path);
}

int schedit_import(SchemeEditor *se, const char *path)
{
    int r = colormgr_load(&se->scheme, path);
    if (r == 0) se->modified = 1;
    return r;
}

void schedit_apply_preview(SchemeEditor *se) { se->preview_active = 1; }
RGB schedit_get_color(SchemeEditor *se, int index) { return colormgr_get(&se->scheme, index); }
