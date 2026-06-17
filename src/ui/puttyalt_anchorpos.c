/* puttyalt_anchorpos.c - Resolve anchored child positions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { ANC_TL, ANC_TR, ANC_BL, ANC_BR, ANC_CENTER };
typedef struct { int x, y; } AncPos;
AncPos anc_resolve(int parent_w, int parent_h, int child_w, int child_h, int anchor, int margin) {
    AncPos p = {margin, margin};
    switch (anchor) {
        case ANC_TR: p.x = parent_w - child_w - margin; p.y = margin; break;
        case ANC_BL: p.x = margin; p.y = parent_h - child_h - margin; break;
        case ANC_BR: p.x = parent_w - child_w - margin; p.y = parent_h - child_h - margin; break;
        case ANC_CENTER: p.x = (parent_w - child_w) / 2; p.y = (parent_h - child_h) / 2; break;
        default: break;
    }
    if (p.x < 0) p.x = 0;
    if (p.y < 0) p.y = 0;
    return p;
}
