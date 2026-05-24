#ifndef PUTTYALT_SCHEDIT_H
#define PUTTYALT_SCHEDIT_H
#include "puttyalt_colormgr.h"

typedef struct {
    ColorScheme scheme;
    int selected_index;
    int preview_active;
    int modified;
    char export_path[512];
} SchemeEditor;

void schedit_init(SchemeEditor *se, const ColorScheme *base);
void schedit_set_color(SchemeEditor *se, int index, RGB color);
void schedit_reset_to_default(SchemeEditor *se);
int  schedit_export(SchemeEditor *se, const char *path);
int  schedit_import(SchemeEditor *se, const char *path);
void schedit_apply_preview(SchemeEditor *se);
RGB  schedit_get_color(SchemeEditor *se, int index);

#endif
