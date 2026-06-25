/* puttyalt_slidingwindow.c - Sliding window for sequence numbers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define SW_SIZE 64
typedef struct { unsigned char received[SW_SIZE]; long base; } SlidingWindow;
void sw3_init(SlidingWindow *w, long base) { if (w) { memset(w->received, 0, sizeof w->received); w->base = base; } }
/* Mark a sequence number received. Returns 0 ok, -1 if outside window. */
int sw3_mark(SlidingWindow *w, long seq) {
    if (!w) return -1;
    if (seq < w->base || seq >= w->base + SW_SIZE) return -1;
    w->received[seq - w->base] = 1;
    return 0;
}
int sw3_is_received(const SlidingWindow *w, long seq) {
    if (!w || seq < w->base || seq >= w->base + SW_SIZE) return 0;
    return w->received[seq - w->base];
}
/* Slide window forward as long as the base is received. Returns new base. */
long sw3_advance(SlidingWindow *w) {
    if (!w) return 0;
    while (w->received[0]) {
        memmove(w->received, w->received + 1, SW_SIZE - 1);
        w->received[SW_SIZE-1] = 0;
        w->base++;
    }
    return w->base;
}
