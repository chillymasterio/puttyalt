/* puttyalt_cellattr.c - Pack/unpack terminal cell attributes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Pack fg(8) bg(8) flags(8) into a 32-bit attribute word. */
uint32_t ca2_pack(int fg, int bg, int bold, int underline, int reverse, int italic) {
    uint32_t flags = 0;
    if (bold) flags |= 1;
    if (underline) flags |= 2;
    if (reverse) flags |= 4;
    if (italic) flags |= 8;
    return ((uint32_t)(fg & 0xFF) << 16) | ((uint32_t)(bg & 0xFF) << 8) | (flags & 0xFF);
}
int ca2_fg(uint32_t a) { return (a >> 16) & 0xFF; }
int ca2_bg(uint32_t a) { return (a >> 8) & 0xFF; }
int ca2_bold(uint32_t a) { return a & 1; }
int ca2_underline(uint32_t a) { return (a >> 1) & 1; }
int ca2_reverse(uint32_t a) { return (a >> 2) & 1; }
int ca2_italic(uint32_t a) { return (a >> 3) & 1; }
