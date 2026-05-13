#include "puttyalt_colorscheme.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void csmgr_init(ColorSchemeMgr *cm)
{
    memset(cm, 0, sizeof(*cm));
    cm->active = -1;
}

int csmgr_add(ColorSchemeMgr *cm, const ColorScheme *scheme)
{
    if (cm->count >= CS_MAX_SCHEMES) return -1;
    cm->schemes[cm->count] = *scheme;
    return cm->count++;
}

int csmgr_remove(ColorSchemeMgr *cm, int index)
{
    if (index < 0 || index >= cm->count) return -1;
    for (int i = index; i < cm->count - 1; i++)
        cm->schemes[i] = cm->schemes[i + 1];
    cm->count--;
    if (cm->active == index) cm->active = -1;
    else if (cm->active > index) cm->active--;
    return 0;
}

int csmgr_set_active(ColorSchemeMgr *cm, int index)
{
    if (index < 0 || index >= cm->count) return -1;
    cm->active = index;
    return 0;
}

const ColorScheme *csmgr_get_active(const ColorSchemeMgr *cm)
{
    if (cm->active < 0 || cm->active >= cm->count) return NULL;
    return &cm->schemes[cm->active];
}

int csmgr_find(const ColorSchemeMgr *cm, const char *name)
{
    for (int i = 0; i < cm->count; i++)
        if (strcmp(cm->schemes[i].name, name) == 0) return i;
    return -1;
}

static void add_scheme(ColorSchemeMgr *cm, const char *name, const char *author,
                       int bg, int fg, int cursor, int is_dark, const int *pal)
{
    ColorScheme s;
    memset(&s, 0, sizeof(s));
    snprintf(s.name, CS_MAX_NAME, "%s", name);
    snprintf(s.author, CS_MAX_NAME, "%s", author);
    s.bg_color = bg; s.fg_color = fg; s.cursor_color = cursor;
    s.is_dark = is_dark;
    s.selection_bg = 0x44475A; s.selection_fg = fg;
    s.bold_color = 0xFFFFFF;
    memcpy(s.palette, pal, CS_PALETTE_SIZE * sizeof(int));
    csmgr_add(cm, &s);
}

void csmgr_load_builtins(ColorSchemeMgr *cm)
{
    /* Warm Blue (default) */
    int wb[] = {0x1B2838,0xE74C3C,0x2ECC71,0xF39C12,0x4A9EE0,0x9B59B6,
                0x1ABC9C,0xC8D6E5,0x34495E,0xFF6B6B,0x55EFC4,0xFECA57,
                0x74B9FF,0xA29BFE,0x00CEC9,0xF5F6FA};
    add_scheme(cm, "Warm Blue", "PuttyAlt", 0x1B2838, 0xC8D6E5, 0x4A9EE0, 1, wb);

    /* Midnight */
    int mn[] = {0x0D1117,0xFF7B72,0x7EE787,0xD29922,0x79C0FF,0xD2A8FF,
                0x56D4DD,0xC9D1D9,0x21262D,0xFFA198,0xAFF5B4,0xE3B341,
                0xA5D6FF,0xE2CBFF,0x76E4F7,0xF0F6FC};
    add_scheme(cm, "Midnight", "PuttyAlt", 0x0D1117, 0xC9D1D9, 0x79C0FF, 1, mn);

    /* Solarized Dark */
    int sd[] = {0x073642,0xDC322F,0x859900,0xB58900,0x268BD2,0xD33682,
                0x2AA198,0xEEE8D5,0x002B36,0xCB4B16,0x586E75,0x657B83,
                0x839496,0x6C71C4,0x93A1A1,0xFDF6E3};
    add_scheme(cm, "Solarized Dark", "Ethan Schoonover", 0x002B36, 0x839496, 0x93A1A1, 1, sd);

    /* Nord */
    int nd[] = {0x3B4252,0xBF616A,0xA3BE8C,0xEBCB8B,0x81A1C1,0xB48EAD,
                0x88C0D0,0xE5E9F0,0x4C566A,0xD08770,0xA3BE8C,0xEBCB8B,
                0x81A1C1,0xB48EAD,0x8FBCBB,0xECEFF4};
    add_scheme(cm, "Nord", "Arctic Ice Studio", 0x2E3440, 0xD8DEE9, 0x88C0D0, 1, nd);

    cm->active = 0;
}

int csmgr_load(ColorSchemeMgr *cm, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    if (!f) return -1;
    csmgr_init(cm);
    ColorScheme *cur = NULL;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
        if (strcmp(line, "[scheme]") == 0) {
            ColorScheme s;
            memset(&s, 0, sizeof(s));
            csmgr_add(cm, &s);
            cur = &cm->schemes[cm->count - 1];
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0) snprintf(cur->name, CS_MAX_NAME, "%s", line+5);
        else if (strncmp(line, "bg=", 3) == 0) cur->bg_color = (int)strtol(line+3, NULL, 16);
        else if (strncmp(line, "fg=", 3) == 0) cur->fg_color = (int)strtol(line+3, NULL, 16);
        else if (strncmp(line, "cursor=", 7) == 0) cur->cursor_color = (int)strtol(line+7, NULL, 16);
    }
    fclose(f);
    return 0;
}

int csmgr_save(const ColorSchemeMgr *cm, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < cm->count; i++) {
        const ColorScheme *s = &cm->schemes[i];
        fprintf(f, "[scheme]\nname=%s\nauthor=%s\nbg=%06X\nfg=%06X\ncursor=%06X\n\n",
                s->name, s->author, s->bg_color, s->fg_color, s->cursor_color);
    }
    fclose(f);
    return 0;
}

int csmgr_export_iterm(const ColorScheme *cs, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\">\n");
    fprintf(f, "<plist version=\"1.0\"><dict>\n");
    fprintf(f, "<key>Background Color</key><dict>\n");
    fprintf(f, "  <key>Red Component</key><real>%f</real>\n", ((cs->bg_color>>16)&0xFF)/255.0);
    fprintf(f, "  <key>Green Component</key><real>%f</real>\n", ((cs->bg_color>>8)&0xFF)/255.0);
    fprintf(f, "  <key>Blue Component</key><real>%f</real>\n", (cs->bg_color&0xFF)/255.0);
    fprintf(f, "</dict>\n</dict></plist>\n");
    fclose(f);
    return 0;
}
