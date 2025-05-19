#ifndef PUTTYALT_COLORPICKER_H
#define PUTTYALT_COLORPICKER_H

typedef struct RGBColor { unsigned char r, g, b; } RGBColor;
typedef struct HSVColor { float h, s, v; } HSVColor;

typedef struct ColorPicker {
    RGBColor current;
    RGBColor recent[16];
    int num_recent;
    int active;
} ColorPicker;

void cp_init(ColorPicker *cp);
void cp_rgb_to_hsv(const RGBColor *rgb, HSVColor *hsv);
void cp_hsv_to_rgb(const HSVColor *hsv, RGBColor *rgb);
unsigned int cp_to_packed(const RGBColor *c);
void cp_from_packed(unsigned int packed, RGBColor *c);
int  cp_parse_hex(const char *hex, RGBColor *c);
void cp_to_hex(const RGBColor *c, char *hex, int hexlen);
void cp_add_recent(ColorPicker *cp, const RGBColor *c);

#endif
