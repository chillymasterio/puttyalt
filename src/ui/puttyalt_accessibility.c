#include "puttyalt_accessibility.h"
#include <string.h>
#include <stdio.h>

void a11y_init(A11yState *a)
{
    memset(a, 0, sizeof(*a));
    a->font_scale = 100;
    a->cursor_blink = 1;
    a->theme = A11Y_THEME_DEFAULT;
}

void a11y_enable(A11yState *a, int enable)
{
    a->enabled = enable;
}

void a11y_set_theme(A11yState *a, A11yTheme theme)
{
    a->theme = theme;
    if (theme == A11Y_THEME_HIGH_CONTRAST || theme == A11Y_THEME_DARK_HC)
        a->focus_highlight = 1;
}

void a11y_set_font_scale(A11yState *a, int pct)
{
    if (pct < 50) pct = 50;
    if (pct > 400) pct = 400;
    a->font_scale = pct;
}

void a11y_announce(A11yState *a, const char *msg)
{
    if (!a->enabled || !a->screen_reader) return;
    snprintf(a->last_announce, A11Y_MAX_ANNOUNCE, "%s", msg);
    /* In real implementation, call platform accessibility API:
     * Windows: UIA RaiseAutomationEvent / NotifyWinEvent
     * macOS: NSAccessibilityPostNotification
     * Linux: ATK / AT-SPI */
}

void a11y_toggle_screen_reader(A11yState *a)
{
    a->screen_reader = !a->screen_reader;
    if (a->screen_reader && !a->enabled)
        a->enabled = 1;
}

void a11y_toggle_visual_bell(A11yState *a)
{
    a->bell_visual = !a->bell_visual;
}

int a11y_get_adjusted_color(const A11yState *a, int color)
{
    if (!a->enabled) return color;

    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;

    switch (a->theme) {
    case A11Y_THEME_HIGH_CONTRAST:
        /* Increase contrast: push toward extremes */
        r = r > 128 ? 255 : 0;
        g = g > 128 ? 255 : 0;
        b = b > 128 ? 255 : 0;
        break;
    case A11Y_THEME_DARK_HC:
        /* Dark high-contrast: bright text colors */
        r = r > 64 ? (r < 200 ? 220 : 255) : 0;
        g = g > 64 ? (g < 200 ? 220 : 255) : 0;
        b = b > 64 ? (b < 200 ? 220 : 255) : 0;
        break;
    case A11Y_THEME_LIGHT_HC:
        r = r < 192 ? (r > 64 ? 40 : 0) : 255;
        g = g < 192 ? (g > 64 ? 40 : 0) : 255;
        b = b < 192 ? (b > 64 ? 40 : 0) : 255;
        break;
    default:
        break;
    }
    return (r << 16) | (g << 8) | b;
}
