/* puttyalt_scrollbackidx.c - Index scrollback for fast line access.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SBI_MAX 4096
typedef struct { int offsets[SBI_MAX]; int count; int total_lines; } ScrollbackIdx;
void sbi_init(ScrollbackIdx *s) { if (s) { s->count = 0; s->total_lines = 0; } }
int sbi_add_line(ScrollbackIdx *s, int byte_offset) {
    if (!s) return -1;
    if (s->count < SBI_MAX) s->offsets[s->count++] = byte_offset;
    else {
        /* ring: drop oldest */
        for (int i = 1; i < SBI_MAX; i++) s->offsets[i-1] = s->offsets[i];
        s->offsets[SBI_MAX-1] = byte_offset;
    }
    s->total_lines++;
    return 0;
}
int sbi_offset(const ScrollbackIdx *s, int line) {
    if (!s || line < 0 || line >= s->count) return -1;
    return s->offsets[line];
}
int sbi_visible_range(const ScrollbackIdx *s, int viewport_top, int height, int *first, int *last) {
    if (!s) return -1;
    int f = viewport_top < 0 ? 0 : viewport_top;
    int l = f + height;
    if (l > s->count) l = s->count;
    if (first) *first = f;
    if (last) *last = l;
    return l - f;
}
