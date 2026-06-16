/* puttyalt_dec_special.c - DEC special graphics character mapping.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Map a DEC Special Graphics char (used for box drawing) to Unicode. */
uint32_t decs_to_unicode(char c) {
    switch (c) {
        case 'j': return 0x2518; /* lower-right corner */
        case 'k': return 0x2510; /* upper-right corner */
        case 'l': return 0x250C; /* upper-left corner */
        case 'm': return 0x2514; /* lower-left corner */
        case 'n': return 0x253C; /* crossing lines */
        case 'q': return 0x2500; /* horizontal line */
        case 't': return 0x251C; /* left tee */
        case 'u': return 0x2524; /* right tee */
        case 'v': return 0x2534; /* bottom tee */
        case 'w': return 0x252C; /* top tee */
        case 'x': return 0x2502; /* vertical line */
        case '`': return 0x25C6; /* diamond */
        case 'a': return 0x2592; /* checkerboard */
        case 'f': return 0x00B0; /* degree */
        case 'g': return 0x00B1; /* plus-minus */
        case '~': return 0x00B7; /* middle dot */
        default:  return (uint32_t)(unsigned char)c;
    }
}
int decs_is_graphic(char c) { return decs_to_unicode(c) != (uint32_t)(unsigned char)c; }
