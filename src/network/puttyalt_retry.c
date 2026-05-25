#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int max_retries;
    int base_delay_ms;
    int max_delay_ms;
    int current_attempt;
    int jitter;
} RetryPolicy;

void retry_init(RetryPolicy *p, int max_retries, int base_delay_ms)
{
    p->max_retries = max_retries > 0 ? max_retries : 5;
    p->base_delay_ms = base_delay_ms > 0 ? base_delay_ms : 1000;
    p->max_delay_ms = 30000;
    p->current_attempt = 0;
    p->jitter = 1;
}

int retry_should_retry(RetryPolicy *p)
{
    return p->current_attempt < p->max_retries;
}

int retry_next_delay(RetryPolicy *p)
{
    int delay = p->base_delay_ms;
    for (int i = 0; i < p->current_attempt && delay < p->max_delay_ms; i++)
        delay *= 2;
    if (delay > p->max_delay_ms) delay = p->max_delay_ms;
    if (p->jitter) delay += rand() % (delay / 4 + 1);
    p->current_attempt++;
    return delay;
}

void retry_reset(RetryPolicy *p) { p->current_attempt = 0; }

int retry_format_status(RetryPolicy *p, char *buf, int buflen)
{
    return snprintf(buf, buflen, "Retry %d/%d (next in %dms)",
                    p->current_attempt, p->max_retries,
                    retry_next_delay(p));
}
