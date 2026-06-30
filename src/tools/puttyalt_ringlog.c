/* puttyalt_ringlog.c - Ring buffer of recent log lines.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define RLG_CAP 32
#define RLG_LEN 128
typedef struct { char lines[RLG_CAP][RLG_LEN]; int head; int count; } RingLog;
void rl5_init(RingLog *r) { if (r) { r->head = 0; r->count = 0; } }
void rl5_push(RingLog *r, const char *line) {
    if (!r || !line) return;
    strncpy(r->lines[r->head], line, RLG_LEN-1); r->lines[r->head][RLG_LEN-1] = 0;
    r->head = (r->head + 1) % RLG_CAP;
    if (r->count < RLG_CAP) r->count++;
}
/* Get the i-th most recent line (0 = newest). */
const char *rl5_recent(const RingLog *r, int i) {
    if (!r || i < 0 || i >= r->count) return 0;
    int idx = (r->head - 1 - i + RLG_CAP * 2) % RLG_CAP;
    return r->lines[idx];
}
