/* puttyalt_colorquant.c - Quantize truecolor to 256-palette.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Map an RGB value to the nearest xterm-256 color index. */
int cq3_to_256(uint8_t r, uint8_t g, uint8_t b) {
    /* grayscale check */
    if (r == g && g == b) {
        if (r < 8) return 16;
        if (r > 248) return 231;
        return 232 + (r - 8) / 10;
    }
    int ri = (r < 48) ? 0 : (r < 115) ? 1 : (r - 35) / 40;
    int gi = (g < 48) ? 0 : (g < 115) ? 1 : (g - 35) / 40;
    int bi = (b < 48) ? 0 : (b < 115) ? 1 : (b - 35) / 40;
    return 16 + 36 * ri + 6 * gi + bi;
}
/* Map to nearest 16-color ANSI index. */
int cq3_to_16(uint8_t r, uint8_t g, uint8_t b) {
    int idx = 0;
    if (r > 128) idx |= 1;
    if (g > 128) idx |= 2;
    if (b > 128) idx |= 4;
    int bright = (r > 192 || g > 192 || b > 192);
    return bright ? idx + 8 : idx;
}
