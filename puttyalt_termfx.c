#include "puttyalt_termfx.h"
#include <string.h>

void termfx_init(TermFX *fx)
{
    memset(fx, 0, sizeof(*fx));
    fx->intensity = 50;
    fx->speed = 16;
    fx->crt_curvature = 10;
    fx->scanline_gap = 2;
    fx->scanline_alpha = 30;
    fx->glow_radius = 2;
    fx->glow_intensity = 20;
}

void termfx_enable(TermFX *fx, FXType type)
{
    fx->type = type;
    fx->enabled = 1;
    fx->progress = 0.0f;
}

void termfx_disable(TermFX *fx)
{
    fx->enabled = 0;
    fx->type = FX_NONE;
}

void termfx_set_intensity(TermFX *fx, int pct)
{
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    fx->intensity = pct;
}

void termfx_set_speed(TermFX *fx, int ms)
{
    if (ms < 1) ms = 1;
    if (ms > 1000) ms = 1000;
    fx->speed = ms;
}

float termfx_update(TermFX *fx, int elapsed_ms)
{
    if (!fx->enabled) return 1.0f;
    fx->progress += (float)elapsed_ms / (float)(fx->speed * 60);
    if (fx->progress > 1.0f) fx->progress = 1.0f;
    return fx->progress;
}

int termfx_apply_crt(int x, int y, int width, int height, int curvature)
{
    /* CRT barrel distortion: offset pixels toward center edges */
    float cx = (float)width / 2.0f;
    float cy = (float)height / 2.0f;
    float dx = ((float)x - cx) / cx;
    float dy = ((float)y - cy) / cy;
    float d2 = dx * dx + dy * dy;
    float factor = 1.0f + d2 * curvature / 100.0f;
    int nx = (int)(cx + dx * factor * cx);
    int ny = (int)(cy + dy * factor * cy);
    if (nx < 0 || nx >= width || ny < 0 || ny >= height) return -1;
    return ny * width + nx;
}

int termfx_scanline_visible(int y, int gap)
{
    return (y % (gap + 1)) != gap;
}

int termfx_apply_glow(int color, int radius, int intensity)
{
    (void)radius;
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    int boost = intensity * 255 / 100;
    r = r + boost > 255 ? 255 : r + boost;
    g = g + boost > 255 ? 255 : g + boost;
    b = b + boost > 255 ? 255 : b + boost;
    return (r << 16) | (g << 8) | b;
}
