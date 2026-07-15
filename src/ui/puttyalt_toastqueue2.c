/* puttyalt_toastqueue2.c - Toast notification queue with priority.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TQ_MAX 8
typedef struct { char text[80]; int priority; long expires; int used; } TqItem;
typedef struct { TqItem items[TQ_MAX]; } ToastQueue;
void tqu_init(ToastQueue *q) { if (q) for (int i = 0; i < TQ_MAX; i++) q->items[i].used = 0; }
int tqu_push(ToastQueue *q, const char *text, int priority, long now, long ttl) {
    if (!q || !text) return -1;
    for (int i = 0; i < TQ_MAX; i++) if (!q->items[i].used) {
        strncpy(q->items[i].text, text, 79); q->items[i].text[79] = 0;
        q->items[i].priority = priority; q->items[i].expires = now + ttl; q->items[i].used = 1;
        return i;
    }
    return -1;
}
int tqu_top(ToastQueue *q, long now) {
    if (!q) return -1;
    int best = -1, bp = -1;
    for (int i = 0; i < TQ_MAX; i++) {
        if (q->items[i].used && now >= q->items[i].expires) q->items[i].used = 0;
        if (q->items[i].used && q->items[i].priority > bp) { bp = q->items[i].priority; best = i; }
    }
    return best;
}
