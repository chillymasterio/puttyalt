/* puttyalt_scrollbar2.c - Compute scrollbar thumb geometry.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int thumb_pos, thumb_size; } ScrollGeom;
/* Given content/viewport/track sizes and scroll offset, compute thumb. */
ScrollGeom sb2_compute(int content, int viewport, int track, int offset) {
    ScrollGeom g = {0, track};
    if (content <= viewport || content <= 0) return g;
    g.thumb_size = (int)((long)viewport * track / content);
    if (g.thumb_size < 8) g.thumb_size = 8;
    if (g.thumb_size > track) g.thumb_size = track;
    int max_off = content - viewport;
    if (offset < 0) offset = 0;
    if (offset > max_off) offset = max_off;
    g.thumb_pos = (int)((long)offset * (track - g.thumb_size) / max_off);
    return g;
}
/* Inverse: map a click at track position back to a scroll offset. */
int sb2_offset_at(int track_y, int thumb_size, int track, int content, int viewport) {
    int max_off = content - viewport;
    if (max_off <= 0 || track <= thumb_size) return 0;
    int o = (int)((long)track_y * max_off / (track - thumb_size));
    if (o < 0) o = 0;
    if (o > max_off) o = max_off;
    return o;
}
