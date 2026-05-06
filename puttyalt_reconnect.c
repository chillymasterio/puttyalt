/*
 * puttyalt_reconnect.c: Auto-reconnect with exponential backoff.
 */

#include "puttyalt_reconnect.h"
#include <string.h>

void reconnect_init(ReconnectCtx *ctx)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->policy = RECONNECT_ON_DROP;
    ctx->state = RSTATE_IDLE;
    ctx->max_attempts = 0;       /* unlimited */
    ctx->attempt = 0;
    ctx->base_delay_ms = 1000;
    ctx->max_delay_ms = 60000;
    ctx->current_delay_ms = 1000;
}

int reconnect_on_disconnect(ReconnectCtx *ctx, int was_clean_exit)
{
    if (ctx->policy == RECONNECT_NEVER)
        return 0;

    if (was_clean_exit && ctx->policy != RECONNECT_ALWAYS)
        return 0;

    if (ctx->max_attempts > 0 && ctx->attempt >= ctx->max_attempts) {
        ctx->state = RSTATE_GAVE_UP;
        return 0;
    }

    ctx->state = RSTATE_WAITING;
    return 1;
}

void reconnect_success(ReconnectCtx *ctx)
{
    ctx->state = RSTATE_CONNECTED;
    ctx->attempt = 0;
    ctx->current_delay_ms = ctx->base_delay_ms;
    ctx->total_reconnects++;
}

void reconnect_failure(ReconnectCtx *ctx)
{
    ctx->attempt++;
    ctx->total_failures++;

    /* Exponential backoff with cap */
    ctx->current_delay_ms *= 2;
    if (ctx->current_delay_ms > ctx->max_delay_ms)
        ctx->current_delay_ms = ctx->max_delay_ms;

    if (ctx->max_attempts > 0 && ctx->attempt >= ctx->max_attempts)
        ctx->state = RSTATE_GAVE_UP;
    else
        ctx->state = RSTATE_WAITING;
}

int reconnect_get_delay(const ReconnectCtx *ctx)
{
    return ctx->current_delay_ms;
}

void reconnect_cancel(ReconnectCtx *ctx)
{
    ctx->state = RSTATE_IDLE;
    ctx->current_delay_ms = ctx->base_delay_ms;
    /* Clear pending timer state to prevent stale timer callbacks */
    ctx->attempt = 0;
}

void reconnect_reset(ReconnectCtx *ctx)
{
    ReconnectPolicy saved_policy = ctx->policy;
    int saved_max = ctx->max_attempts;
    int saved_base = ctx->base_delay_ms;
    int saved_cap = ctx->max_delay_ms;

    reconnect_init(ctx);

    ctx->policy = saved_policy;
    ctx->max_attempts = saved_max;
    ctx->base_delay_ms = saved_base;
    ctx->max_delay_ms = saved_cap;
    ctx->current_delay_ms = saved_base;
}
