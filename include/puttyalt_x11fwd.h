#ifndef PUTTYALT_X11FWD_H
#define PUTTYALT_X11FWD_H

typedef struct X11ForwardConfig {
    int enabled;
    int trusted;             /* trusted vs untrusted forwarding */
    char display[128];       /* DISPLAY value override */
    char xauth_path[512];    /* path to .Xauthority */
    int timeout_sec;         /* auto-expire forwarding */
    unsigned long started_at;
} X11ForwardConfig;

void x11fwd_init(X11ForwardConfig *cfg);
int  x11fwd_start(X11ForwardConfig *cfg);
int  x11fwd_stop(X11ForwardConfig *cfg);
int  x11fwd_is_expired(const X11ForwardConfig *cfg, unsigned long now);
const char *x11fwd_get_display(const X11ForwardConfig *cfg);

#endif
