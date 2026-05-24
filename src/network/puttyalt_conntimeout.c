#include "puttyalt_conntimeout.h"
#include <string.h>

void timeout_config_default(TimeoutConfig *tc)
{
    memset(tc, 0, sizeof(*tc));
    tc->connect_timeout_sec = 30;
    tc->auth_timeout_sec = 60;
    tc->idle_timeout_sec = 0; /* disabled */
    tc->keepalive_interval_sec = 60;
    tc->keepalive_max_retries = 3;
    tc->tcp_nodelay = 1;
    tc->network_timeout_sec = 15;
    tc->warn_before_idle_disconnect = 1;
    tc->warn_seconds_before = 30;
}

void timeout_config_fast(TimeoutConfig *tc)
{
    timeout_config_default(tc);
    tc->connect_timeout_sec = 10;
    tc->auth_timeout_sec = 30;
    tc->keepalive_interval_sec = 30;
    tc->network_timeout_sec = 5;
}

void timeout_config_patient(TimeoutConfig *tc)
{
    timeout_config_default(tc);
    tc->connect_timeout_sec = 120;
    tc->auth_timeout_sec = 180;
    tc->keepalive_interval_sec = 120;
    tc->network_timeout_sec = 60;
    tc->keepalive_max_retries = 10;
}

int timeout_is_expired(TimeoutConfig *tc, int elapsed_sec, int type)
{
    int limit = 0;
    switch (type) {
        case 0: limit = tc->connect_timeout_sec; break;
        case 1: limit = tc->auth_timeout_sec; break;
        case 2: limit = tc->idle_timeout_sec; break;
        case 3: limit = tc->network_timeout_sec; break;
        default: return 0;
    }
    return (limit > 0 && elapsed_sec >= limit);
}
