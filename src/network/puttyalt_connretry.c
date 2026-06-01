/* puttyalt_connretry.c - Connection retry with jittered exponential backoff. */
#include <string.h>
#include <stdint.h>
typedef struct { int base_ms, max_ms, attempt, max_attempts; uint32_t seed; } ConnRetry;
void connretry_init(ConnRetry *r, int base_ms, int max_ms, int max_attempts) {
    if(!r) return; memset(r,0,sizeof(*r));
    r->base_ms=base_ms>0?base_ms:500; r->max_ms=max_ms>0?max_ms:30000;
    r->max_attempts=max_attempts>0?max_attempts:10; r->seed=0x12345;
}
int connretry_next_delay(ConnRetry *r) {
    if(!r||r->attempt>=r->max_attempts) return -1;
    int exp=r->base_ms; for (int i=0;i<r->attempt && exp<r->max_ms;i++) exp*=2;
    if (exp>r->max_ms) exp=r->max_ms;
    r->seed = r->seed*1103515245u + 12345u;
    int jitter = (int)((r->seed>>16) % (exp/4 + 1));
    r->attempt++;
    return exp - exp/8 + jitter;
}
void connretry_reset(ConnRetry *r) { if(r) r->attempt=0; }
int connretry_attempts(const ConnRetry *r) { return r?r->attempt:-1; }
int connretry_exhausted(const ConnRetry *r) { return r? (r->attempt>=r->max_attempts):1; }
