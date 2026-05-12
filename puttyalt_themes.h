#ifndef PUTTYALT_THEMES_H
#define PUTTYALT_THEMES_H

#define THEME_MAX_NAME    64
#define THEME_MAX_THEMES  32
#define THEME_MAX_COLORS  24

typedef struct {
    char name[THEME_MAX_NAME];
    int  colors[THEME_MAX_COLORS];
    int  font_size;
    int  cursor_shape;      /* 0=block, 1=line, 2=underline */
    int  opacity;           /* 0-255 */
    int  bold_as_bright;
    int  dim_factor;        /* percentage */
    int  builtin;
} Theme;

typedef struct {
    Theme themes[THEME_MAX_THEMES];
    int   count;
    int   active;
    int   preview;          /* -1 = none */
} ThemeEngine;

/* Standard ANSI color indices */
enum {
    TC_BG = 0, TC_FG, TC_CURSOR, TC_SELECTION,
    TC_BLACK, TC_RED, TC_GREEN, TC_YELLOW,
    TC_BLUE, TC_MAGENTA, TC_CYAN, TC_WHITE,
    TC_BRIGHT_BLACK, TC_BRIGHT_RED, TC_BRIGHT_GREEN, TC_BRIGHT_YELLOW,
    TC_BRIGHT_BLUE, TC_BRIGHT_MAGENTA, TC_BRIGHT_CYAN, TC_BRIGHT_WHITE,
    TC_SIDEBAR_BG, TC_SIDEBAR_FG, TC_TAB_ACTIVE, TC_TAB_INACTIVE
};

void theme_engine_init(ThemeEngine *te);
int  theme_add(ThemeEngine *te, const char *name);
int  theme_set_color(ThemeEngine *te, int theme_idx, int color_idx, int rgb);
int  theme_activate(ThemeEngine *te, int index);
int  theme_preview(ThemeEngine *te, int index);
void theme_cancel_preview(ThemeEngine *te);
int  theme_get_color(const ThemeEngine *te, int color_idx);
int  theme_find(const ThemeEngine *te, const char *name);
int  theme_remove(ThemeEngine *te, int index);
int  theme_load(ThemeEngine *te, const char *path);
int  theme_save(const ThemeEngine *te, const char *path);
void theme_install_defaults(ThemeEngine *te);

#endif
