/* puttyalt_retrpolicy.c - Retry policy with circuit breaker. */
#include <string.h>
#include <stdint.h>
enum rp_circuit { RP_CLOSED = 0, RP_OPEN = 1, RP_HALF_OPEN = 2 };
typedef struct {
    int max_failures;
    int reset_timeout_ms;
    int failure_count;
    int success_count;
    int circuit_state;
    uint64_t opened_at;
} RetrPolicy;
void retrpolicy_init(RetrPolicy *r, int max_failures, int reset_timeout_ms) {
    if (!r) return;
    memset(r, 0, sizeof(*r));
    r->max_failures = max_failures > 0 ? max_failures : 5;
    r->reset_timeout_ms = reset_timeout_ms > 0 ? reset_timeout_ms : 30000;
}
int retrpolicy_allow(RetrPolicy *r, uint64_t now_ms) {
    if (!r) return 0;
    if (r->circuit_state == RP_OPEN) {
        if ((now_ms - r->opened_at) >= (uint64_t)r->reset_timeout_ms) {
            r->circuit_state = RP_HALF_OPEN;
            return 1;
        }
        return 0;
    }
    return 1;
}
void retrpolicy_record(RetrPolicy *r, int success, uint64_t now_ms) {
    if (!r) return;
    if (success) {
        r->success_count++;
        if (r->circuit_state == RP_HALF_OPEN) {
            r->circuit_state = RP_CLOSED;
            r->failure_count = 0;
        }
    } else {
        r->failure_count++;
        if (r->circuit_state == RP_HALF_OPEN) {
            r->circuit_state = RP_OPEN;
            r->opened_at = now_ms;
        } else if (r->failure_count >= r->max_failures) {
            r->circuit_state = RP_OPEN;
            r->opened_at = now_ms;
        }
    }
}
int retrpolicy_state(const RetrPolicy *r) {
    return r ? r->circuit_state : -1;
}
