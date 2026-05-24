#ifndef PUTTYALT_COLORSCHEME_H
#define PUTTYALT_COLORSCHEME_H

#define CS_MAX_SCHEMES  32
#define CS_MAX_NAME     64
#define CS_PALETTE_SIZE 16

typedef struct {
    char  name[CS_MAX_NAME];
    char  author[CS_MAX_NAME];
    int   palette[CS_PALETTE_SIZE];  /* ANSI 0-15 */
    int   bg_color;
    int   fg_color;
    int   cursor_color;
    int   selection_bg;
    int   selection_fg;
    int   bold_color;
    int   is_dark;
} ColorScheme;

typedef struct {
    ColorScheme schemes[CS_MAX_SCHEMES];
    int         count;
    int         active;
} ColorSchemeMgr;

void csmgr_init(ColorSchemeMgr *cm);
int  csmgr_add(ColorSchemeMgr *cm, const ColorScheme *scheme);
int  csmgr_remove(ColorSchemeMgr *cm, int index);
int  csmgr_set_active(ColorSchemeMgr *cm, int index);
const ColorScheme *csmgr_get_active(const ColorSchemeMgr *cm);
int  csmgr_find(const ColorSchemeMgr *cm, const char *name);
void csmgr_load_builtins(ColorSchemeMgr *cm);
int  csmgr_load(ColorSchemeMgr *cm, const char *path);
int  csmgr_save(const ColorSchemeMgr *cm, const char *path);
int  csmgr_export_iterm(const ColorScheme *cs, const char *path);

#endif
