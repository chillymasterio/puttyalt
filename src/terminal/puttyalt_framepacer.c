/* puttyalt_framepacer.c - Frame pacing with target FPS budget.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long frame_budget_us; long last_frame; int dropped; int rendered; } FramePacer;
void fp4_init(FramePacer *p, int target_fps) {
    if (!p) return;
    p->frame_budget_us = target_fps > 0 ? 1000000L / target_fps : 16667;
    p->last_frame = 0; p->dropped = 0; p->rendered = 0;
}
/* Should we render a frame now? Enforces the budget. */
int fp4_should_render(FramePacer *p, long now_us) {
    if (!p) return 1;
    if (now_us - p->last_frame >= p->frame_budget_us) {
        p->last_frame = now_us; p->rendered++;
        return 1;
    }
    p->dropped++;
    return 0;
}
int fp4_actual_fps(const FramePacer *p, long elapsed_us) {
    if (!p || elapsed_us <= 0) return 0;
    return (int)((long)p->rendered * 1000000L / elapsed_us);
}
