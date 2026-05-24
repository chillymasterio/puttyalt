#include <string.h>
#include <stdlib.h>
#include "puttyalt_x11fwd.h"

void x11fwd_init(X11ForwardConfig *cfg)
{
    memset(cfg, 0, sizeof(*cfg));
    cfg->timeout_sec = 3600; /* 1 hour default */
    /* Try to get DISPLAY from environment */
    const char *disp = getenv("DISPLAY");
    if (disp) strncpy(cfg->display, disp, sizeof(cfg->display) - 1);
}

int x11fwd_start(X11ForwardConfig *cfg)
{
    if (cfg->enabled) return 0; /* already active */
    cfg->enabled = 1;
    return 0;
}

int x11fwd_stop(X11ForwardConfig *cfg)
{
    cfg->enabled = 0;
    return 0;
}

int x11fwd_is_expired(const X11ForwardConfig *cfg, unsigned long now)
{
    if (!cfg->enabled || cfg->timeout_sec <= 0) return 0;
    return (now - cfg->started_at) > (unsigned long)cfg->timeout_sec;
}

const char *x11fwd_get_display(const X11ForwardConfig *cfg)
{
    return cfg->display[0] ? cfg->display : ":0";
}
