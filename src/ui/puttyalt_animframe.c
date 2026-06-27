/* puttyalt_animframe.c - Frame-based animation interpolation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int total_frames; int current; int loop; } AnimFrame;
void af2_init(AnimFrame *a, int total_frames, int loop) {
    if (!a) return;
    a->total_frames = total_frames > 0 ? total_frames : 1;
    a->current = 0; a->loop = loop;
}
/* Advance; returns 1 if still animating. */
int af2_tick(AnimFrame *a) {
    if (!a) return 0;
    a->current++;
    if (a->current >= a->total_frames) {
        if (a->loop) { a->current = 0; return 1; }
        a->current = a->total_frames; return 0;
    }
    return 1;
}
/* Progress 0..1000 (fixed point). */
int af2_progress(const AnimFrame *a) {
    if (!a || a->total_frames == 0) return 1000;
    return a->current * 1000 / a->total_frames;
}
