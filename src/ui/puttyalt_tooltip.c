/* puttyalt_tooltip.c - Position tooltips to stay on screen.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int x, y; } TipPos;
/* Place a tooltip near (mx,my), flipping to stay within the screen. */
TipPos ttp_place(int mx, int my, int tip_w, int tip_h, int screen_w, int screen_h, int offset) {
    TipPos p;
    p.x = mx + offset;
    p.y = my + offset;
    if (p.x + tip_w > screen_w) p.x = mx - tip_w - offset;
    if (p.y + tip_h > screen_h) p.y = my - tip_h - offset;
    if (p.x < 0) p.x = 0;
    if (p.y < 0) p.y = 0;
    return p;
}
