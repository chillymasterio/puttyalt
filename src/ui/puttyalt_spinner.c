/* puttyalt_spinner.c - Cycle spinner animation frames.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
static const char *SPIN_FRAMES[] = { "|", "/", "-", "\\" };
typedef struct { int frame; } Spinner;
void spin_init(Spinner *s) { if (s) s->frame = 0; }
const char *spin_next(Spinner *s) {
    if (!s) return "";
    const char *f = SPIN_FRAMES[s->frame & 3];
    s->frame = (s->frame + 1) & 3;
    return f;
}
const char *spin_dots(int tick) {
    static const char *d[] = { ".  ", ".. ", "...", " ..", "  .", "   " };
    return d[((tick % 6) + 6) % 6];
}
