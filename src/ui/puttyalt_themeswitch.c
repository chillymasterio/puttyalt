#include "puttyalt_themeswitch.h"
#include "puttyalt_design.h"
#include <string.h>

int theme_reg_init(ThemeRegistry *tr)
{
    memset(tr, 0, sizeof(*tr));
    /* Built-in themes */
    ThemeDef github_dark = {"GitHub Dark", DS_BG, DS_SURFACE, DS_BORDER,
                            DS_TEXT, DS_TEXT_SECONDARY, DS_ACCENT, DS_SUCCESS, DS_ERROR};
    ThemeDef monokai = {"Monokai", 0x272822, 0x2D2E27, 0x3E3F39,
                        0xF8F8F2, 0x75715E, 0xA6E22E, 0xA6E22E, 0xF92672};
    ThemeDef nord = {"Nord", 0x2E3440, 0x3B4252, 0x434C5E,
                     0xECEFF4, 0x7B88A1, 0x88C0D0, 0xA3BE8C, 0xBF616A};
    ThemeDef solarized = {"Solarized Dark", 0x002B36, 0x073642, 0x586E75,
                          0xFDF6E3, 0x93A1A1, 0x268BD2, 0x859900, 0xDC322F};
    theme_reg_add(tr, &github_dark);
    theme_reg_add(tr, &monokai);
    theme_reg_add(tr, &nord);
    theme_reg_add(tr, &solarized);
    return 0;
}

int theme_reg_add(ThemeRegistry *tr, const ThemeDef *t)
{
    if (tr->count >= THEME_MAX) return -1;
    tr->themes[tr->count++] = *t;
    return tr->count - 1;
}

int theme_reg_activate(ThemeRegistry *tr, int idx)
{
    if (idx < 0 || idx >= tr->count) return -1;
    tr->active = idx;
    return 0;
}

const ThemeDef *theme_reg_current(const ThemeRegistry *tr)
{
    return &tr->themes[tr->active];
}
