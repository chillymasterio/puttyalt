#include "puttyalt_themes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void theme_engine_init(ThemeEngine *te)
{
    memset(te, 0, sizeof(*te));
    te->active = -1;
    te->preview = -1;
    theme_install_defaults(te);
}

int theme_add(ThemeEngine *te, const char *name)
{
    if (te->count >= THEME_MAX_THEMES) return -1;
    Theme *t = &te->themes[te->count];
    memset(t, 0, sizeof(*t));
    snprintf(t->name, THEME_MAX_NAME, "%s", name);
    t->font_size = 11;
    t->opacity = 255;
    t->dim_factor = 50;
    return te->count++;
}

int theme_set_color(ThemeEngine *te, int theme_idx, int color_idx, int rgb)
{
    if (theme_idx < 0 || theme_idx >= te->count) return -1;
    if (color_idx < 0 || color_idx >= THEME_MAX_COLORS) return -1;
    te->themes[theme_idx].colors[color_idx] = rgb;
    return 0;
}

int theme_activate(ThemeEngine *te, int index)
{
    if (index < 0 || index >= te->count) return -1;
    te->active = index;
    te->preview = -1;
    return 0;
}

int theme_preview(ThemeEngine *te, int index)
{
    if (index < 0 || index >= te->count) return -1;
    te->preview = index;
    return 0;
}

void theme_cancel_preview(ThemeEngine *te)
{
    te->preview = -1;
}

int theme_get_color(const ThemeEngine *te, int color_idx)
{
    if (color_idx < 0 || color_idx >= THEME_MAX_COLORS) return 0;
    int idx = te->preview >= 0 ? te->preview : te->active;
    if (idx < 0 || idx >= te->count) return 0;
    return te->themes[idx].colors[color_idx];
}

int theme_find(const ThemeEngine *te, const char *name)
{
    for (int i = 0; i < te->count; i++)
        if (strcmp(te->themes[i].name, name) == 0) return i;
    return -1;
}

int theme_remove(ThemeEngine *te, int index)
{
    if (index < 0 || index >= te->count) return -1;
    if (te->themes[index].builtin) return -1;
    for (int i = index; i < te->count - 1; i++)
        te->themes[i] = te->themes[i + 1];
    te->count--;
    if (te->active >= te->count) te->active = te->count - 1;
    return 0;
}

int theme_load(ThemeEngine *te, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[256];
    Theme *cur = NULL;
    if (!f) return -1;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[theme]") == 0) {
            int idx = theme_add(te, "unnamed");
            if (idx < 0) break;
            cur = &te->themes[idx];
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, THEME_MAX_NAME, "%s", line + 5);
        else if (strncmp(line, "color", 5) == 0) {
            char *eq = strchr(line, '=');
            if (eq) {
                int ci = atoi(line + 5);
                int cv = (int)strtol(eq + 1, NULL, 16);
                if (ci >= 0 && ci < THEME_MAX_COLORS)
                    cur->colors[ci] = cv;
            }
        }
        else if (strncmp(line, "font_size=", 10) == 0)
            cur->font_size = atoi(line + 10);
        else if (strncmp(line, "opacity=", 8) == 0)
            cur->opacity = atoi(line + 8);
    }
    fclose(f);
    return 0;
}

int theme_save(const ThemeEngine *te, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < te->count; i++) {
        const Theme *t = &te->themes[i];
        if (t->builtin) continue;
        fprintf(f, "[theme]\nname=%s\nfont_size=%d\nopacity=%d\n",
                t->name, t->font_size, t->opacity);
        for (int c = 0; c < THEME_MAX_COLORS; c++)
            fprintf(f, "color%d=%06X\n", c, t->colors[c]);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

void theme_install_defaults(ThemeEngine *te)
{
    /* Warm Blue (default) */
    int idx = theme_add(te, "Warm Blue");
    if (idx >= 0) {
        Theme *t = &te->themes[idx];
        t->builtin = 1;
        t->colors[TC_BG] = 0x1B2838;
        t->colors[TC_FG] = 0xC8D6E5;
        t->colors[TC_CURSOR] = 0x4A9EE0;
        t->colors[TC_SELECTION] = 0x2A4A6B;
        t->colors[TC_BLACK] = 0x0D1520;
        t->colors[TC_RED] = 0xE74C3C;
        t->colors[TC_GREEN] = 0x2ECC71;
        t->colors[TC_YELLOW] = 0xF39C12;
        t->colors[TC_BLUE] = 0x4A9EE0;
        t->colors[TC_MAGENTA] = 0x9B59B6;
        t->colors[TC_CYAN] = 0x1ABC9C;
        t->colors[TC_WHITE] = 0xC8D6E5;
        t->colors[TC_SIDEBAR_BG] = 0x14202E;
        t->colors[TC_SIDEBAR_FG] = 0x8899AA;
        t->colors[TC_TAB_ACTIVE] = 0x1B2838;
        t->colors[TC_TAB_INACTIVE] = 0x14202E;
        te->active = idx;
    }

    /* Ocean Dark */
    idx = theme_add(te, "Ocean Dark");
    if (idx >= 0) {
        Theme *t = &te->themes[idx];
        t->builtin = 1;
        t->colors[TC_BG] = 0x0A1628;
        t->colors[TC_FG] = 0xA3B8CC;
        t->colors[TC_CURSOR] = 0x5CB3FF;
        t->colors[TC_SELECTION] = 0x1A3350;
        t->colors[TC_BLACK] = 0x050D18;
        t->colors[TC_RED] = 0xFF6B6B;
        t->colors[TC_GREEN] = 0x6BCB77;
        t->colors[TC_YELLOW] = 0xFFD93D;
        t->colors[TC_BLUE] = 0x5CB3FF;
        t->colors[TC_MAGENTA] = 0xC77DFF;
        t->colors[TC_CYAN] = 0x4ECDC4;
        t->colors[TC_WHITE] = 0xD4E4F1;
        t->colors[TC_SIDEBAR_BG] = 0x070F1C;
        t->colors[TC_SIDEBAR_FG] = 0x6B8099;
        t->colors[TC_TAB_ACTIVE] = 0x0A1628;
        t->colors[TC_TAB_INACTIVE] = 0x070F1C;
    }

    /* Midnight Green */
    idx = theme_add(te, "Midnight Green");
    if (idx >= 0) {
        Theme *t = &te->themes[idx];
        t->builtin = 1;
        t->colors[TC_BG] = 0x0B1E1E;
        t->colors[TC_FG] = 0xB8D8D0;
        t->colors[TC_CURSOR] = 0x50E3C2;
        t->colors[TC_SELECTION] = 0x1A3A35;
        t->colors[TC_BLACK] = 0x061212;
        t->colors[TC_RED] = 0xF06292;
        t->colors[TC_GREEN] = 0x50E3C2;
        t->colors[TC_YELLOW] = 0xFFE082;
        t->colors[TC_BLUE] = 0x4FC3F7;
        t->colors[TC_MAGENTA] = 0xCE93D8;
        t->colors[TC_CYAN] = 0x50E3C2;
        t->colors[TC_WHITE] = 0xD0EBE5;
        t->colors[TC_SIDEBAR_BG] = 0x081414;
        t->colors[TC_SIDEBAR_FG] = 0x6B9990;
        t->colors[TC_TAB_ACTIVE] = 0x0B1E1E;
        t->colors[TC_TAB_INACTIVE] = 0x081414;
    }

    /* Classic PuTTY */
    idx = theme_add(te, "Classic PuTTY");
    if (idx >= 0) {
        Theme *t = &te->themes[idx];
        t->builtin = 1;
        t->colors[TC_BG] = 0x000000;
        t->colors[TC_FG] = 0xBBBBBB;
        t->colors[TC_CURSOR] = 0x00FF00;
        t->colors[TC_SELECTION] = 0x444444;
        t->colors[TC_BLACK] = 0x000000;
        t->colors[TC_RED] = 0xBB0000;
        t->colors[TC_GREEN] = 0x00BB00;
        t->colors[TC_YELLOW] = 0xBBBB00;
        t->colors[TC_BLUE] = 0x0000BB;
        t->colors[TC_MAGENTA] = 0xBB00BB;
        t->colors[TC_CYAN] = 0x00BBBB;
        t->colors[TC_WHITE] = 0xBBBBBB;
        t->colors[TC_SIDEBAR_BG] = 0x1A1A1A;
        t->colors[TC_SIDEBAR_FG] = 0x888888;
        t->colors[TC_TAB_ACTIVE] = 0x000000;
        t->colors[TC_TAB_INACTIVE] = 0x1A1A1A;
    }
}
