/* puttyalt_linedraw.c - Box drawing / line drawing character mapper. */
#include <stdint.h>
/* map ASCII line-draw chars (DEC graphics) to Unicode box drawing */
uint32_t linedraw_map(char c) {
    switch(c) {
        case 'j': return 0x2518; /* lower-right corner */
        case 'k': return 0x2510; /* upper-right corner */
        case 'l': return 0x250C; /* upper-left corner */
        case 'm': return 0x2514; /* lower-left corner */
        case 'n': return 0x253C; /* crossing lines */
        case 'q': return 0x2500; /* horizontal line */
        case 't': return 0x251C; /* left T */
        case 'u': return 0x2524; /* right T */
        case 'v': return 0x2534; /* bottom T */
        case 'w': return 0x252C; /* top T */
        case 'x': return 0x2502; /* vertical line */
        case '`': return 0x25C6; /* diamond */
        case 'a': return 0x2592; /* checkerboard */
        case 'f': return 0x00B0; /* degree */
        case 'g': return 0x00B1; /* plus-minus */
        case '~': return 0x00B7; /* bullet */
        default:  return (uint32_t)c;
    }
}
int linedraw_is_drawable(char c) { return (c>='`'&&c<='~')?1:0; }
