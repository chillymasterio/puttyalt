/* puttyalt_bom.c - Detect and strip byte-order marks.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { BOM_NONE, BOM_UTF8, BOM_UTF16LE, BOM_UTF16BE, BOM_UTF32LE, BOM_UTF32BE };
int bom_detect(const unsigned char *d, int n) {
    if (n >= 4 && d[0]==0 && d[1]==0 && d[2]==0xFE && d[3]==0xFF) return BOM_UTF32BE;
    if (n >= 4 && d[0]==0xFF && d[1]==0xFE && d[2]==0 && d[3]==0) return BOM_UTF32LE;
    if (n >= 3 && d[0]==0xEF && d[1]==0xBB && d[2]==0xBF) return BOM_UTF8;
    if (n >= 2 && d[0]==0xFE && d[1]==0xFF) return BOM_UTF16BE;
    if (n >= 2 && d[0]==0xFF && d[1]==0xFE) return BOM_UTF16LE;
    return BOM_NONE;
}
int bom_length(int type) {
    switch (type) {
        case BOM_UTF8: return 3;
        case BOM_UTF16LE: case BOM_UTF16BE: return 2;
        case BOM_UTF32LE: case BOM_UTF32BE: return 4;
        default: return 0;
    }
}
