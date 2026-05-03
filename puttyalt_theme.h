/*
 * puttyalt_theme.h: Terminal colour theme system.
 *
 * Load and switch between colour schemes (Solarized, Dracula, Nord,
 * Monokai, etc.) without editing the session config.
 */

#ifndef PUTTYALT_THEME_H
#define PUTTYALT_THEME_H

#define MAX_THEMES      64
#define THEME_COLORS    22   /* PuTTY uses 22 configurable colours */
#define MAX_THEME_NAME  64

typedef struct {
    unsigned char r, g, b;
} ThemeColour;

typedef struct Theme {
    char         name[MAX_THEME_NAME];
    ThemeColour  colours[THEME_COLORS];
    int          bold_as_bright;
    char         font_name[128];
    int          font_size;
} Theme;

typedef struct ThemeManager {
    Theme themes[MAX_THEMES];
    int   count;
    int   active;       /* currently applied theme index, -1 = none */
} ThemeManager;

void theme_mgr_init(ThemeManager *tm);
int  theme_load_file(ThemeManager *tm, const char *path);
int  theme_load_dir(ThemeManager *tm, const char *dir);
int  theme_add_builtin_solarized_dark(ThemeManager *tm);
int  theme_add_builtin_dracula(ThemeManager *tm);
int  theme_add_builtin_nord(ThemeManager *tm);
int  theme_add_builtin_monokai(ThemeManager *tm);
int  theme_add_builtin_gruvbox(ThemeManager *tm);
int  theme_apply(ThemeManager *tm, int index);
int  theme_find(const ThemeManager *tm, const char *name);
const Theme *theme_get_active(const ThemeManager *tm);

#endif /* PUTTYALT_THEME_H */
