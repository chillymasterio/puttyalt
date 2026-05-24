#ifndef PUTTYALT_ACCESSIBILITY_H
#define PUTTYALT_ACCESSIBILITY_H

#define A11Y_MAX_ANNOUNCE   512

typedef enum {
    A11Y_THEME_DEFAULT = 0,
    A11Y_THEME_HIGH_CONTRAST,
    A11Y_THEME_DARK_HC,
    A11Y_THEME_LIGHT_HC
} A11yTheme;

typedef struct {
    int        enabled;
    int        screen_reader;
    A11yTheme  theme;
    int        large_cursor;
    int        cursor_blink;
    int        font_scale;       /* percentage, 100 = normal */
    int        line_spacing;     /* extra pixels between lines */
    int        bell_visual;      /* flash screen instead of sound */
    int        announce_output;  /* announce new terminal output */
    int        focus_highlight;
    char       last_announce[A11Y_MAX_ANNOUNCE];
} A11yState;

void a11y_init(A11yState *a);
void a11y_enable(A11yState *a, int enable);
void a11y_set_theme(A11yState *a, A11yTheme theme);
void a11y_set_font_scale(A11yState *a, int pct);
void a11y_announce(A11yState *a, const char *msg);
void a11y_toggle_screen_reader(A11yState *a);
void a11y_toggle_visual_bell(A11yState *a);
int  a11y_get_adjusted_color(const A11yState *a, int color);

#endif
