/* puttyalt_charpicker.c - Special character picker categories.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <stddef.h>
/* Return a representative codepoint for a category + index. */
uint32_t cpk2_arrow(int i) {
    static const uint32_t arrows[] = {0x2190,0x2191,0x2192,0x2193,0x2194,0x2195,0x21B5,0x21BB};
    return (i >= 0 && i < 8) ? arrows[i] : 0x2192;
}
uint32_t cpk2_box(int i) {
    static const uint32_t box[] = {0x2500,0x2502,0x250C,0x2510,0x2514,0x2518,0x251C,0x2524};
    return (i >= 0 && i < 8) ? box[i] : 0x2500;
}
uint32_t cpk2_math(int i) {
    static const uint32_t math[] = {0x00B1,0x00D7,0x00F7,0x2260,0x2264,0x2265,0x221E,0x2211};
    return (i >= 0 && i < 8) ? math[i] : 0x00B1;
}
