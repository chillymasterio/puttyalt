#include "puttyalt_cfgvalid.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static void add_msg(ValidationResult *r, const char *fmt, ...)
{
    if (r->msg_count >= 32) return;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(r->messages[r->msg_count++], 256, fmt, ap);
    va_end(ap);
}

void cfg_validate(const GUIConfig *cfg, ValidationResult *result)
{
    memset(result, 0, sizeof(*result));
    if (cfg->width < GUI_MIN_WIDTH) { add_msg(result, "Width too small: %d", cfg->width); result->warning_count++; }
    if (cfg->height < GUI_MIN_HEIGHT) { add_msg(result, "Height too small: %d", cfg->height); result->warning_count++; }
    if (cfg->font_size < 6 || cfg->font_size > 72) { add_msg(result, "Font size out of range: %d", cfg->font_size); result->error_count++; }
    if (cfg->scrollback_lines < 100) { add_msg(result, "Scrollback too small: %d", cfg->scrollback_lines); result->warning_count++; }
    if (cfg->scrollback_lines > 1000000) { add_msg(result, "Scrollback too large: %d", cfg->scrollback_lines); result->warning_count++; }
    if (cfg->transparency < 0 || cfg->transparency > 255) { add_msg(result, "Transparency out of range: %d", cfg->transparency); result->error_count++; }
    if (cfg->sidebar_width < 100 || cfg->sidebar_width > 600) { add_msg(result, "Sidebar width unusual: %d", cfg->sidebar_width); result->warning_count++; }
}

int cfg_validate_host(const char *host)
{
    if (!host || !host[0]) return 0;
    for (int i = 0; host[i]; i++) {
        char c = host[i];
        if (!isalnum(c) && c != '.' && c != '-' && c != ':' && c != '[' && c != ']') return 0;
    }
    return 1;
}

int cfg_validate_port(int port) { return port >= 1 && port <= 65535; }
int cfg_validate_font(const char *name, int size) { return name && name[0] && size >= 6 && size <= 72; }
int cfg_validate_color(unsigned int color) { return color <= 0xFFFFFF; }

void cfg_sanitize(GUIConfig *cfg)
{
    if (cfg->width < GUI_MIN_WIDTH) cfg->width = GUI_DEFAULT_WIDTH;
    if (cfg->height < GUI_MIN_HEIGHT) cfg->height = GUI_DEFAULT_HEIGHT;
    if (cfg->font_size < 6) cfg->font_size = 11;
    if (cfg->font_size > 72) cfg->font_size = 11;
    if (cfg->scrollback_lines < 100) cfg->scrollback_lines = 20000;
    if (cfg->transparency < 0) cfg->transparency = 0;
    if (cfg->transparency > 255) cfg->transparency = 255;
    if (cfg->sidebar_width < 100) cfg->sidebar_width = 220;
}
