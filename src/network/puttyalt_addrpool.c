/* puttyalt_addrpool.c - Address pool with health-based selection. */
#include <string.h>
#include <stdio.h>
#define AP_MAX 16
#define AP_ADDR 48
typedef struct {
    char addr[AP_ADDR];
    int healthy;
    int latency_ms;
    int fail_count;
} ap_addr;
typedef struct {
    ap_addr addrs[AP_MAX];
    int n;
    int rr_index;
} AddrPool;
void addrpool_init(AddrPool *a) {
    if (a) memset(a, 0, sizeof(*a));
}
int addrpool_add(AddrPool *a, const char *addr) {
    if (!a || a->n >= AP_MAX || !addr) return -1;
    ap_addr *ad = &a->addrs[a->n];
    snprintf(ad->addr, AP_ADDR, "%s", addr);
    ad->healthy = 1;
    ad->latency_ms = 100;
    return a->n++;
}
void addrpool_report(AddrPool *a, int idx, int success, int latency_ms) {
    if (!a || idx < 0 || idx >= a->n) return;
    if (success) {
        a->addrs[idx].healthy = 1;
        a->addrs[idx].latency_ms = latency_ms;
        a->addrs[idx].fail_count = 0;
    } else {
        if (++a->addrs[idx].fail_count >= 3) a->addrs[idx].healthy = 0;
    }
}
int addrpool_best(const AddrPool *a) {
    if (!a) return -1;
    int best = -1;
    for (int i = 0; i < a->n; i++) {
        if (!a->addrs[i].healthy) continue;
        if (best < 0 || a->addrs[i].latency_ms < a->addrs[best].latency_ms) best = i;
    }
    return best;
}
int addrpool_round_robin(AddrPool *a) {
    if (!a || a->n == 0) return -1;
    for (int k = 0; k < a->n; k++) {
        int i = (a->rr_index + k) % a->n;
        if (a->addrs[i].healthy) {
            a->rr_index = (i + 1) % a->n;
            return i;
        }
    }
    return -1;
}
int addrpool_healthy_count(const AddrPool *a) {
    if (!a) return -1;
    int n = 0;
    for (int i = 0; i < a->n; i++)
        if (a->addrs[i].healthy) n++;
    return n;
}
