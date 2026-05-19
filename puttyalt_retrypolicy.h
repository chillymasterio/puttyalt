#ifndef PUTTYALT_RETRYPOLICY_H
#define PUTTYALT_RETRYPOLICY_H

typedef struct {
    int  max_retries;
    int  initial_delay_ms;
    int  max_delay_ms;
    double backoff_factor;
    int  jitter_pct;
    int  current_retry;
    int  current_delay_ms;
    int  total_retries;
    int  reset_on_success;
} RetryPolicy;

void retry_policy_default(RetryPolicy *rp);
void retry_policy_aggressive(RetryPolicy *rp);
void retry_policy_conservative(RetryPolicy *rp);
int  retry_next_delay(RetryPolicy *rp);
void retry_reset(RetryPolicy *rp);
int  retry_should_retry(RetryPolicy *rp);

#endif
