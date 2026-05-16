/*
 * puttyalt_theme.c: Theme engine implementation with built-in schemes.
 */

#include "puttyalt_theme.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void theme_mgr_init(ThemeManager *tm)
{
    memset(tm, 0, sizeof(*tm));
    tm->active = -1;
}

static int add_theme(ThemeManager *tm, const Theme *t)
{
    if (tm->count >= MAX_THEMES)
        return -1;
    memcpy(&tm->themes[tm->count], t, sizeof(Theme));
    return tm->count++;
}

int theme_add_builtin_solarized_dark(ThemeManager *tm)
{
    Theme t = {0};
    snprintf(t.name, sizeof(t.name), "Solarized Dark");
    /* Base03 through Base3 + accent colours */
    t.colours[0] = (ThemeColour){0, 43, 54};       /* default fg -> base0 */
    t.colours[1] = (ThemeColour){131, 148, 150};
    t.colours[2] = (ThemeColour){0, 43, 54};       /* default bg -> base03 */
    t.colours[3] = (ThemeColour){7, 54, 66};
    t.colours[4] = (ThemeColour){220, 50, 47};     /* ANSI red */
    t.colours[5] = (ThemeColour){203, 75, 22};
    t.colours[6] = (ThemeColour){133, 153, 0};     /* ANSI green */
    t.colours[7] = (ThemeColour){88, 110, 117};
    t.colours[8] = (ThemeColour){181, 137, 0};     /* ANSI yellow */
    t.colours[9] = (ThemeColour){101, 123, 131};
    t.colours[10] = (ThemeColour){38, 139, 210};   /* ANSI blue */
    t.colours[11] = (ThemeColour){131, 148, 150};
    t.colours[12] = (ThemeColour){211, 54, 130};   /* ANSI magenta */
    t.colours[13] = (ThemeColour){108, 113, 196};
    t.colours[14] = (ThemeColour){42, 161, 152};   /* ANSI cyan */
    t.colours[15] = (ThemeColour){147, 161, 161};
    t.bold_as_bright = 0;
    return add_theme(tm, &t);
}

int theme_add_builtin_dracula(ThemeManager *tm)
{
    Theme t = {0};
    snprintf(t.name, sizeof(t.name), "Dracula");
    t.colours[0] = (ThemeColour){248, 248, 242};   /* fg */
    t.colours[1] = (ThemeColour){248, 248, 242};
    t.colours[2] = (ThemeColour){40, 42, 54};      /* bg */
    t.colours[3] = (ThemeColour){68, 71, 90};
    t.colours[4] = (ThemeColour){255, 85, 85};     /* red */
    t.colours[5] = (ThemeColour){255, 110, 110};
    t.colours[6] = (ThemeColour){80, 250, 123};    /* green */
    t.colours[7] = (ThemeColour){105, 255, 148};
    t.colours[8] = (ThemeColour){241, 250, 140};   /* yellow */
    t.colours[9] = (ThemeColour){255, 255, 165};
    t.colours[10] = (ThemeColour){189, 147, 249};  /* blue/purple */
    t.colours[11] = (ThemeColour){214, 172, 255};
    t.colours[12] = (ThemeColour){255, 121, 198};  /* magenta/pink */
    t.colours[13] = (ThemeColour){255, 146, 208};
    t.colours[14] = (ThemeColour){139, 233, 253};  /* cyan */
    t.colours[15] = (ThemeColour){164, 255, 255};
    t.bold_as_bright = 1;
    return add_theme(tm, &t);
}

int theme_add_builtin_nord(ThemeManager *tm)
{
    Theme t = {0};
    snprintf(t.name, sizeof(t.name), "Nord");
    t.colours[0] = (ThemeColour){216, 222, 233};   /* fg */
    t.colours[1] = (ThemeColour){229, 233, 240};
    t.colours[2] = (ThemeColour){46, 52, 64};      /* bg */
    t.colours[3] = (ThemeColour){59, 66, 82};
    t.colours[4] = (ThemeColour){191, 97, 106};    /* red */
    t.colours[5] = (ThemeColour){191, 97, 106};
    t.colours[6] = (ThemeColour){163, 190, 140};   /* green */
    t.colours[7] = (ThemeColour){163, 190, 140};
    t.colours[8] = (ThemeColour){235, 203, 139};   /* yellow */
    t.colours[9] = (ThemeColour){235, 203, 139};
    t.colours[10] = (ThemeColour){129, 161, 193};  /* blue */
    t.colours[11] = (ThemeColour){129, 161, 193};
    t.colours[12] = (ThemeColour){180, 142, 173};  /* magenta */
    t.colours[13] = (ThemeColour){180, 142, 173};
    t.colours[14] = (ThemeColour){136, 192, 208};  /* cyan */
    t.colours[15] = (ThemeColour){143, 188, 187};
    t.bold_as_bright = 0;
    return add_theme(tm, &t);
}

int theme_add_builtin_monokai(ThemeManager *tm)
{
    Theme t = {0};
    snprintf(t.name, sizeof(t.name), "Monokai");
    t.colours[0] = (ThemeColour){248, 248, 242};
    t.colours[1] = (ThemeColour){248, 248, 242};
    t.colours[2] = (ThemeColour){39, 40, 34};
    t.colours[3] = (ThemeColour){73, 72, 62};
    t.colours[4] = (ThemeColour){249, 38, 114};
    t.colours[5] = (ThemeColour){249, 38, 114};
    t.colours[6] = (ThemeColour){166, 226, 46};
    t.colours[7] = (ThemeColour){166, 226, 46};
    t.colours[8] = (ThemeColour){244, 191, 117};
    t.colours[9] = (ThemeColour){244, 191, 117};
    t.colours[10] = (ThemeColour){102, 217, 239};
    t.colours[11] = (ThemeColour){102, 217, 239};
    t.colours[12] = (ThemeColour){174, 129, 255};
    t.colours[13] = (ThemeColour){174, 129, 255};
    t.colours[14] = (ThemeColour){161, 239, 228};
    t.colours[15] = (ThemeColour){161, 239, 228};
    t.bold_as_bright = 1;
    return add_theme(tm, &t);
}

int theme_add_builtin_gruvbox(ThemeManager *tm)
{
    Theme t = {0};
    snprintf(t.name, sizeof(t.name), "Gruvbox Dark");
    t.colours[0] = (ThemeColour){235, 219, 178};
    t.colours[1] = (ThemeColour){251, 241, 199};
    t.colours[2] = (ThemeColour){40, 40, 40};
    t.colours[3] = (ThemeColour){60, 56, 54};
    t.colours[4] = (ThemeColour){204, 36, 29};
    t.colours[5] = (ThemeColour){251, 73, 52};
    t.colours[6] = (ThemeColour){152, 151, 26};
    t.colours[7] = (ThemeColour){184, 187, 38};
    t.colours[8] = (ThemeColour){215, 153, 33};
    t.colours[9] = (ThemeColour){250, 189, 47};
    t.colours[10] = (ThemeColour){69, 133, 136};
    t.colours[11] = (ThemeColour){131, 165, 152};
    t.colours[12] = (ThemeColour){177, 98, 134};
    t.colours[13] = (ThemeColour){211, 134, 155};
    t.colours[14] = (ThemeColour){104, 157, 106};
    t.colours[15] = (ThemeColour){142, 192, 124};
    t.bold_as_bright = 0;
    return add_theme(tm, &t);
}

int theme_apply(ThemeManager *tm, int index)
{
    if (index < 0 || index >= tm->count)
        return -1;
    tm->active = index;
    return 0;
}

int theme_find(const ThemeManager *tm, const char *name)
{
    for (int i = 0; i < tm->count; i++) {
        if (strcmp(tm->themes[i].name, name) == 0)
            return i;
    }
    return -1;
}

const Theme *theme_get_active(const ThemeManager *tm)
{
    if (tm->active < 0 || tm->active >= tm->count)
        return NULL;
    return &tm->themes[tm->active];
}

int theme_load_file(ThemeManager *tm, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[256];
    Theme t = {0};
    int color_idx = 0;

    if (!f) return -1;

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        if (strncmp(line, "name=", 5) == 0)
            snprintf(t.name, sizeof(t.name), "%s", line + 5);
        else if (strncmp(line, "color", 5) == 0) {
            unsigned int r, g, b;
            char *eq = strchr(line, '=');
            if (eq && sscanf(eq + 1, "%u,%u,%u", &r, &g, &b) == 3) {
                if (color_idx < THEME_COLORS) {
                    t.colours[color_idx].r = (unsigned char)r;
                    t.colours[color_idx].g = (unsigned char)g;
                    t.colours[color_idx].b = (unsigned char)b;
                    color_idx++;
                }
            }
        }
    }

    fclose(f);
    if (t.name[0])
        return add_theme(tm, &t);
    return -1;
}

int theme_load_dir(ThemeManager *tm, const char *dir)
{
    (void)tm;
    (void)dir;
    /* Platform-specific directory iteration would go here */
    return 0;
}
