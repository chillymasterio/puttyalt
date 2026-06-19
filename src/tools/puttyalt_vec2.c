/* puttyalt_vec2.c - 2D integer vector math.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int x, y; } Vec2;
Vec2 v2_add(Vec2 a, Vec2 b) { Vec2 r = {a.x+b.x, a.y+b.y}; return r; }
Vec2 v2_sub(Vec2 a, Vec2 b) { Vec2 r = {a.x-b.x, a.y-b.y}; return r; }
Vec2 v2_scale(Vec2 a, int s) { Vec2 r = {a.x*s, a.y*s}; return r; }
long v2_dot(Vec2 a, Vec2 b) { return (long)a.x*b.x + (long)a.y*b.y; }
long v2_cross(Vec2 a, Vec2 b) { return (long)a.x*b.y - (long)a.y*b.x; }
long v2_manhattan(Vec2 a, Vec2 b) {
    int dx = a.x-b.x, dy = a.y-b.y;
    return (dx<0?-dx:dx) + (dy<0?-dy:dy);
}
long v2_dist2(Vec2 a, Vec2 b) {
    long dx = a.x-b.x, dy = a.y-b.y;
    return dx*dx + dy*dy;
}
