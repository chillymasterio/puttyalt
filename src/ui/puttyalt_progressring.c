/* puttyalt_progressring.c - Circular progress ring renderer (segments). */
#include <string.h>
typedef struct {
    int percent;
    int segments;
    int filled_segments;
} ProgressRing;
void progressring_init(ProgressRing *p, int segments) {
    if (!p) return;
    memset(p, 0, sizeof(*p));
    p->segments = segments > 0 ? segments : 12;
}
void progressring_set(ProgressRing *p, int percent) {
    if (!p) return;
    p->percent = percent < 0 ? 0 : (percent > 100 ? 100 : percent);
    p->filled_segments = p->percent * p->segments / 100;
}
int progressring_segment_filled(const ProgressRing *p, int seg) {
    if (!p || seg < 0 || seg >= p->segments) return 0;
    return seg < p->filled_segments ? 1 : 0;
}
int progressring_percent(const ProgressRing *p) {
    return p ? p->percent : -1;
}
int progressring_is_complete(const ProgressRing *p) {
    return (p && p->percent >= 100) ? 1 : 0;
}
