#ifndef PUTTYALT_TERMFX_H
#define PUTTYALT_TERMFX_H

typedef enum {
    FX_NONE = 0,
    FX_FADE_IN,
    FX_SLIDE_LEFT,
    FX_TYPING,
    FX_MATRIX,
    FX_CRT,
    FX_SCANLINES
} FXType;

typedef struct {
    FXType  type;
    int     intensity;     /* 0-100 */
    int     speed;         /* ms per frame */
    int     enabled;
    float   progress;      /* 0.0 - 1.0 */
    int     crt_curvature;
    int     crt_flicker;
    int     scanline_gap;
    int     scanline_alpha;
    int     glow_radius;
    int     glow_intensity;
} TermFX;

void termfx_init(TermFX *fx);
void termfx_enable(TermFX *fx, FXType type);
void termfx_disable(TermFX *fx);
void termfx_set_intensity(TermFX *fx, int pct);
void termfx_set_speed(TermFX *fx, int ms);
float termfx_update(TermFX *fx, int elapsed_ms);
int  termfx_apply_crt(int x, int y, int width, int height, int curvature);
int  termfx_scanline_visible(int y, int gap);
int  termfx_apply_glow(int color, int radius, int intensity);

#endif
