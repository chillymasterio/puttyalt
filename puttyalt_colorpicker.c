#include <string.h>
#include <stdio.h>
#include <math.h>
#include "puttyalt_colorpicker.h"

void cp_init(ColorPicker *cp)
{
    memset(cp, 0, sizeof(*cp));
    cp->current.r = cp->current.g = cp->current.b = 255;
}

void cp_rgb_to_hsv(const RGBColor *rgb, HSVColor *hsv)
{
    float r = rgb->r / 255.0f, g = rgb->g / 255.0f, b = rgb->b / 255.0f;
    float cmax = r > g ? (r > b ? r : b) : (g > b ? g : b);
    float cmin = r < g ? (r < b ? r : b) : (g < b ? g : b);
    float diff = cmax - cmin;
    hsv->v = cmax;
    hsv->s = (cmax == 0) ? 0 : diff / cmax;
    if (diff == 0) { hsv->h = 0; return; }
    if (cmax == r) hsv->h = fmodf((g - b) / diff, 6.0f) * 60.0f;
    else if (cmax == g) hsv->h = ((b - r) / diff + 2.0f) * 60.0f;
    else hsv->h = ((r - g) / diff + 4.0f) * 60.0f;
    if (hsv->h < 0) hsv->h += 360.0f;
}

void cp_hsv_to_rgb(const HSVColor *hsv, RGBColor *rgb)
{
    float c = hsv->v * hsv->s;
    float x = c * (1 - fabsf(fmodf(hsv->h / 60.0f, 2) - 1));
    float m = hsv->v - c;
    float r, g, b;
    if (hsv->h < 60) { r=c; g=x; b=0; }
    else if (hsv->h < 120) { r=x; g=c; b=0; }
    else if (hsv->h < 180) { r=0; g=c; b=x; }
    else if (hsv->h < 240) { r=0; g=x; b=c; }
    else if (hsv->h < 300) { r=x; g=0; b=c; }
    else { r=c; g=0; b=x; }
    rgb->r = (unsigned char)((r + m) * 255);
    rgb->g = (unsigned char)((g + m) * 255);
    rgb->b = (unsigned char)((b + m) * 255);
}

unsigned int cp_to_packed(const RGBColor *c)
{ return ((unsigned int)c->r << 16) | ((unsigned int)c->g << 8) | c->b; }

void cp_from_packed(unsigned int p, RGBColor *c)
{ c->r = (p >> 16) & 0xFF; c->g = (p >> 8) & 0xFF; c->b = p & 0xFF; }

int cp_parse_hex(const char *hex, RGBColor *c)
{
    if (!hex) return -1;
    if (hex[0] == '#') hex++;
    if (strlen(hex) != 6) return -1;
    unsigned int val;
    if (sscanf(hex, "%06x", &val) != 1) return -1;
    cp_from_packed(val, c);
    return 0;
}

void cp_to_hex(const RGBColor *c, char *hex, int hexlen)
{ snprintf(hex, hexlen, "#%02X%02X%02X", c->r, c->g, c->b); }

void cp_add_recent(ColorPicker *cp, const RGBColor *c)
{
    if (cp->num_recent >= 16) cp->num_recent = 15;
    for (int i = cp->num_recent; i > 0; i--) cp->recent[i] = cp->recent[i-1];
    cp->recent[0] = *c;
    if (cp->num_recent < 16) cp->num_recent++;
}
