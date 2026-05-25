#include <string.h>
#include <stdio.h>

#define MAX_SCHEMES 32
#define SCHEME_NAME_LEN 64
#define COLOR_COUNT 16

typedef struct {
    char name[SCHEME_NAME_LEN];
    unsigned int colors[COLOR_COUNT]; /* RRGGBB */
    unsigned int bg;
    unsigned int fg;
    unsigned int cursor;
} ColorScheme;

static ColorScheme g_schemes[MAX_SCHEMES];
static int g_scheme_count = 0;
static int g_active = -1;

static void add_builtin(const char *name, unsigned int bg, unsigned int fg, unsigned int cursor)
{
    if (g_scheme_count >= MAX_SCHEMES) return;
    ColorScheme *s = &g_schemes[g_scheme_count];
    snprintf(s->name, SCHEME_NAME_LEN, "%s", name);
    s->bg = bg; s->fg = fg; s->cursor = cursor;
    g_scheme_count++;
}

void colorschemes_init(void)
{
    g_scheme_count = 0;
    add_builtin("Warm Blue",    0x1a1b2e, 0xd4d4d4, 0x58a6ff);
    add_builtin("Dracula",      0x282a36, 0xf8f8f2, 0xff79c6);
    add_builtin("Solarized",    0x002b36, 0x839496, 0x268bd2);
    add_builtin("Nord",         0x2e3440, 0xd8dee9, 0x88c0d0);
    add_builtin("Gruvbox",      0x282828, 0xebdbb2, 0xfe8019);
    add_builtin("One Dark",     0x282c34, 0xabb2bf, 0x61afef);
    add_builtin("Tokyo Night",  0x1a1b26, 0xa9b1d6, 0x7aa2f7);
    add_builtin("Catppuccin",   0x1e1e2e, 0xcdd6f4, 0xf5c2e7);
    add_builtin("Monokai",      0x272822, 0xf8f8f2, 0xf92672);
    add_builtin("GitHub Dark",  0x0d1117, 0xc9d1d9, 0x58a6ff);
    g_active = 0;
}

int colorschemes_set(const char *name)
{
    for (int i = 0; i < g_scheme_count; i++)
        if (strcmp(g_schemes[i].name, name) == 0) { g_active = i; return 0; }
    return -1;
}

const char *colorschemes_active_name(void)
{
    return g_active >= 0 ? g_schemes[g_active].name : "Default";
}

int colorschemes_list(char names[][SCHEME_NAME_LEN], int max)
{
    int n = g_scheme_count < max ? g_scheme_count : max;
    for (int i = 0; i < n; i++)
        snprintf(names[i], SCHEME_NAME_LEN, "%s", g_schemes[i].name);
    return n;
}

unsigned int colorschemes_get_bg(void)  { return g_active >= 0 ? g_schemes[g_active].bg : 0x1a1b2e; }
unsigned int colorschemes_get_fg(void)  { return g_active >= 0 ? g_schemes[g_active].fg : 0xd4d4d4; }
unsigned int colorschemes_get_cursor(void) { return g_active >= 0 ? g_schemes[g_active].cursor : 0x58a6ff; }
