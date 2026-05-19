#include "puttyalt_retrypolicy.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void retry_policy_default(RetryPolicy *rp)
{
    memset(rp, 0, sizeof(*rp));
    rp->max_retries = 5;
    rp->initial_delay_ms = 1000;
    rp->max_delay_ms = 30000;
    rp->backoff_factor = 2.0;
    rp->jitter_pct = 20;
    rp->reset_on_success = 1;
}

void retry_policy_aggressive(RetryPolicy *rp)
{
    retry_policy_default(rp);
    rp->max_retries = 20;
    rp->initial_delay_ms = 500;
    rp->max_delay_ms = 10000;
    rp->backoff_factor = 1.5;
}

void retry_policy_conservative(RetryPolicy *rp)
{
    retry_policy_default(rp);
    rp->max_retries = 3;
    rp->initial_delay_ms = 5000;
    rp->max_delay_ms = 60000;
    rp->backoff_factor = 3.0;
}

int retry_next_delay(RetryPolicy *rp)
{
    if (rp->current_retry == 0) {
        rp->current_delay_ms = rp->initial_delay_ms;
    } else {
        rp->current_delay_ms = (int)(rp->current_delay_ms * rp->backoff_factor);
        if (rp->current_delay_ms > rp->max_delay_ms)
            rp->current_delay_ms = rp->max_delay_ms;
    }
    /* add jitter */
    if (rp->jitter_pct > 0) {
        int jitter_range = rp->current_delay_ms * rp->jitter_pct / 100;
        if (jitter_range > 0) {
            srand((unsigned)time(NULL) ^ rp->current_retry);
            int jitter = (rand() % (jitter_range * 2 + 1)) - jitter_range;
            rp->current_delay_ms += jitter;
        }
    }
    rp->current_retry++;
    rp->total_retries++;
    return rp->current_delay_ms;
}

void retry_reset(RetryPolicy *rp) { rp->current_retry = 0; rp->current_delay_ms = 0; }
int retry_should_retry(RetryPolicy *rp) { return rp->current_retry < rp->max_retries; }
