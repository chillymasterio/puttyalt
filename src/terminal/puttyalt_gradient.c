/* puttyalt_gradient.c - Generate color gradients across cells.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Interpolate between two colors across `steps`; write step `i`. */
uint32_t gr2_at(uint32_t start, uint32_t end, int steps, int i) {
    if (steps <= 1) return start;
    if (i < 0) i = 0;
    if (i >= steps) i = steps - 1;
    int rs=(start>>16)&255, gs=(start>>8)&255, bs=start&255;
    int re=(end>>16)&255, ge=(end>>8)&255, be=end&255;
    int r = rs + (re-rs)*i/(steps-1);
    int g = gs + (ge-gs)*i/(steps-1);
    int b = bs + (be-bs)*i/(steps-1);
    return (uint32_t)((r<<16)|(g<<8)|b);
}
int gr2_fill(uint32_t start, uint32_t end, uint32_t *out, int steps) {
    if (!out || steps <= 0) return -1;
    for (int i = 0; i < steps; i++) out[i] = gr2_at(start, end, steps, i);
    return 0;
}
