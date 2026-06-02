/* puttyalt_retryctx.c - Retry context with per-error-class policies. */
#include <string.h>
#include <stdio.h>
#define RX_MAX 8
typedef struct { char err_class[32]; int max_retries; int backoff_base_ms; int retryable; } rx_policy;
typedef struct { rx_policy p[RX_MAX]; int n; int attempts; char current_err[32]; } RetryCtx;
void retryctx_init(RetryCtx *r) { if(r) memset(r,0,sizeof(*r)); }
int retryctx_policy(RetryCtx *r, const char *err_class, int max_retries, int backoff_base_ms, int retryable) {
    if(!r||r->n>=RX_MAX||!err_class) return -1;
    rx_policy *p=&r->p[r->n++]; snprintf(p->err_class,32,"%s",err_class);
    p->max_retries=max_retries; p->backoff_base_ms=backoff_base_ms; p->retryable=retryable?1:0; return 0;
}
int retryctx_should_retry(RetryCtx *r, const char *err_class) {
    if(!r||!err_class) return 0;
    snprintf(r->current_err,32,"%s",err_class);
    for (int i=0;i<r->n;i++) if (strcmp(r->p[i].err_class,err_class)==0) {
        if (!r->p[i].retryable) return 0;
        return r->attempts<r->p[i].max_retries?1:0;
    }
    return 0;
}
int retryctx_next_backoff(RetryCtx *r) {
    if(!r) return -1;
    for (int i=0;i<r->n;i++) if (strcmp(r->p[i].err_class,r->current_err)==0) {
        int b=r->p[i].backoff_base_ms; for (int k=0;k<r->attempts;k++) b*=2;
        r->attempts++; return b;
    }
    return -1;
}
void retryctx_reset(RetryCtx *r) { if(r) r->attempts=0; }
