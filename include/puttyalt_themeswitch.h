#ifndef PUTTYALT_THEMESWITCH_H
#define PUTTYALT_THEMESWITCH_H

#define THEME_MAX 16
typedef struct {
    char name[32];
    unsigned int bg, surface, border, text, text_dim, accent, success, error;
} ThemeDef;
typedef struct {
    ThemeDef themes[THEME_MAX];
    int count;
    int active;
} ThemeRegistry;

int  theme_reg_init(ThemeRegistry *tr);
int  theme_reg_add(ThemeRegistry *tr, const ThemeDef *t);
int  theme_reg_activate(ThemeRegistry *tr, int idx);
const ThemeDef *theme_reg_current(const ThemeRegistry *tr);

#endif
