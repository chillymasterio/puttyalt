/* puttyalt_rectops.c - Rectangle intersection/union/contains.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int x, y, w, h; } Rect2;
int rc2_contains(Rect2 r, int px, int py) {
    return px >= r.x && px < r.x + r.w && py >= r.y && py < r.y + r.h;
}
int rc2_intersects(Rect2 a, Rect2 b) {
    return a.x < b.x + b.w && b.x < a.x + a.w && a.y < b.y + b.h && b.y < a.y + a.h;
}
Rect2 rc2_intersection(Rect2 a, Rect2 b) {
    Rect2 r;
    int x0 = a.x > b.x ? a.x : b.x;
    int y0 = a.y > b.y ? a.y : b.y;
    int x1 = (a.x+a.w) < (b.x+b.w) ? (a.x+a.w) : (b.x+b.w);
    int y1 = (a.y+a.h) < (b.y+b.h) ? (a.y+a.h) : (b.y+b.h);
    r.x = x0; r.y = y0; r.w = x1 > x0 ? x1-x0 : 0; r.h = y1 > y0 ? y1-y0 : 0;
    return r;
}
Rect2 rc2_union(Rect2 a, Rect2 b) {
    Rect2 r;
    int x0 = a.x < b.x ? a.x : b.x;
    int y0 = a.y < b.y ? a.y : b.y;
    int x1 = (a.x+a.w) > (b.x+b.w) ? (a.x+a.w) : (b.x+b.w);
    int y1 = (a.y+a.h) > (b.y+b.h) ? (a.y+a.h) : (b.y+b.h);
    r.x = x0; r.y = y0; r.w = x1-x0; r.h = y1-y0;
    return r;
}
int rc2_area(Rect2 r) { return r.w > 0 && r.h > 0 ? r.w * r.h : 0; }
