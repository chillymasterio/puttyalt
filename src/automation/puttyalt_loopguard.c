/* puttyalt_loopguard.c - Loop guard against runaway automation (iteration cap). */
#include <string.h>
#include <stdint.h>
typedef struct {
    int max_iterations;
    int max_duration_ms;
    int iterations;
    uint64_t start_ms;
    int tripped;
} LoopGuard;
void loopguard_init(LoopGuard *l, int max_iter, int max_duration_ms, uint64_t now_ms) {
    if (!l) return;
    memset(l, 0, sizeof(*l));
    l->max_iterations = max_iter > 0 ? max_iter : 10000;
    l->max_duration_ms = max_duration_ms > 0 ? max_duration_ms : 30000;
    l->start_ms = now_ms;
}
int loopguard_check(LoopGuard *l, uint64_t now_ms) {
    if (!l) return -1;
    l->iterations++;
    if (l->iterations >= l->max_iterations) {
        l->tripped = 1;
        return 1;
    }
    if ((now_ms - l->start_ms) >= (uint64_t)l->max_duration_ms) {
        l->tripped = 1;
        return 1;
    }
    return 0;
}
void loopguard_reset(LoopGuard *l, uint64_t now_ms) {
    if (!l) return;
    l->iterations = 0;
    l->start_ms = now_ms;
    l->tripped = 0;
}
int loopguard_is_tripped(const LoopGuard *l) {
    return l ? l->tripped : -1;
}
int loopguard_iterations(const LoopGuard *l) {
    return l ? l->iterations : -1;
}
