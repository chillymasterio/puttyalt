/* puttyalt_sessionpool.c - Pool and reuse session connection slots.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SP_CAP 32
typedef struct { int session_id; int in_use; long last_used; } SpSlot;
typedef struct { SpSlot slots[SP_CAP]; } SessionPool;
void sp3_init(SessionPool *p) { if (p) for (int i=0;i<SP_CAP;i++) { p->slots[i].in_use = 0; p->slots[i].session_id = -1; } }
int sp3_acquire(SessionPool *p, int session_id, long now) {
    if (!p) return -1;
    for (int i = 0; i < SP_CAP; i++) if (!p->slots[i].in_use) {
        p->slots[i].in_use = 1; p->slots[i].session_id = session_id; p->slots[i].last_used = now;
        return i;
    }
    return -1;
}
int sp3_release(SessionPool *p, int slot) {
    if (!p || slot < 0 || slot >= SP_CAP) return -1;
    p->slots[slot].in_use = 0;
    return 0;
}
int sp3_active(const SessionPool *p) {
    if (!p) return 0;
    int c = 0;
    for (int i = 0; i < SP_CAP; i++) if (p->slots[i].in_use) c++;
    return c;
}
