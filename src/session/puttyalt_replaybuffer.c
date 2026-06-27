/* puttyalt_replaybuffer.c - Buffer terminal output for session replay.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define RP_CAP 4096
typedef struct { char data[RP_CAP]; int len; int overflow; } ReplayBuffer;
void rb3_init(ReplayBuffer *r) { if (r) { r->len = 0; r->overflow = 0; } }
int rb3_write(ReplayBuffer *r, const char *data, int n) {
    if (!r || !data) return -1;
    for (int i = 0; i < n; i++) {
        if (r->len >= RP_CAP) { r->overflow = 1; return -1; }
        r->data[r->len++] = data[i];
    }
    return 0;
}
int rb3_size(const ReplayBuffer *r) { return r ? r->len : 0; }
void rb3_clear(ReplayBuffer *r) { if (r) { r->len = 0; r->overflow = 0; } }
/* Find the byte offset of line number `n` (0-based). */
int rb3_line_offset(const ReplayBuffer *r, int line) {
    if (!r) return -1;
    if (line == 0) return 0;
    int cur = 0;
    for (int i = 0; i < r->len; i++) if (r->data[i] == '\n') { cur++; if (cur == line) return i + 1; }
    return -1;
}
