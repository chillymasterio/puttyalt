/* puttyalt_vsyncctl.c - present-mode / vsync-tear control for PuttyAlt.
 * Self-contained C99 module. No project headers, no windows.h. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define VSYNCCTL_HZ_MIN  24u
#define VSYNCCTL_HZ_MAX  360u

typedef enum {
    VSYNCCTL_MODE_FIFO = 0,      /* vsync-on, no tearing */
    VSYNCCTL_MODE_MAILBOX,       /* triple-buffered */
    VSYNCCTL_MODE_IMMEDIATE      /* tearing permitted */
} vsyncctl_mode_t;

typedef struct {
    int             initialized;
    vsyncctl_mode_t mode;
    uint32_t        refresh_hz;
    uint32_t        interval_ms;     /* derived from refresh_hz */
    int             tearing_allowed;
    int             have_last;
    uint64_t        last_present_ms;
    uint64_t        present_count;
} vsyncctl_state_t;

static vsyncctl_state_t g_vsyncctl;

static uint32_t vsyncctl_clamp_hz(uint32_t hz)
{
    if (hz < VSYNCCTL_HZ_MIN) return VSYNCCTL_HZ_MIN;
    if (hz > VSYNCCTL_HZ_MAX) return VSYNCCTL_HZ_MAX;
    return hz;
}

static void vsyncctl_recalc(uint32_t refresh_hz)
{
    g_vsyncctl.refresh_hz  = vsyncctl_clamp_hz(refresh_hz);
    g_vsyncctl.interval_ms = 1000u / g_vsyncctl.refresh_hz;
    if (g_vsyncctl.interval_ms == 0u) g_vsyncctl.interval_ms = 1u;
}

int vsyncctl_init(uint32_t refresh_hz)
{
    memset(&g_vsyncctl, 0, sizeof(g_vsyncctl));
    vsyncctl_recalc(refresh_hz);
    g_vsyncctl.mode        = VSYNCCTL_MODE_FIFO;
    g_vsyncctl.initialized = 1;
    return 0;
}

int vsyncctl_set_mode(vsyncctl_mode_t mode)
{
    if (!g_vsyncctl.initialized) return -1;
    if (mode != VSYNCCTL_MODE_FIFO &&
        mode != VSYNCCTL_MODE_MAILBOX &&
        mode != VSYNCCTL_MODE_IMMEDIATE)
        return -1;
    g_vsyncctl.mode = mode;
    if (mode == VSYNCCTL_MODE_IMMEDIATE)
        g_vsyncctl.tearing_allowed = 1;
    return 0;
}

int vsyncctl_set_refresh_hz(uint32_t refresh_hz)
{
    if (!g_vsyncctl.initialized) return -1;
    vsyncctl_recalc(refresh_hz);
    return 0;
}

int vsyncctl_allow_tearing(int allow)
{
    if (!g_vsyncctl.initialized) return -1;
    g_vsyncctl.tearing_allowed = allow ? 1 : 0;
    return 0;
}

int vsyncctl_should_present(uint64_t now_ms)
{
    uint64_t elapsed;
    if (!g_vsyncctl.initialized) return -1;
    if (!g_vsyncctl.have_last) return 1;
    /* first frame */
    if (now_ms <= g_vsyncctl.last_present_ms) return 0;
    elapsed = now_ms - g_vsyncctl.last_present_ms;
    /* Immediate (or tearing override) presents as soon as any time passed. */
    if (g_vsyncctl.mode == VSYNCCTL_MODE_IMMEDIATE || g_vsyncctl.tearing_allowed)
        return 1;
    return (elapsed >= (uint64_t)g_vsyncctl.interval_ms) ? 1 : 0;
}

int vsyncctl_mark_presented(uint64_t now_ms)
{
    if (!g_vsyncctl.initialized) return -1;
    g_vsyncctl.last_present_ms = now_ms;
    g_vsyncctl.have_last       = 1;
    g_vsyncctl.present_count++;
    return 0;
}

vsyncctl_mode_t vsyncctl_active_mode(void)
{
    return g_vsyncctl.mode;
}

int vsyncctl_describe(char *buf, size_t len)
{
    const char *names[] = { "fifo", "mailbox", "immediate" };
    const char *name;
    int n;
    if (buf == NULL || len == 0u) return -1;
    name = (g_vsyncctl.mode <= VSYNCCTL_MODE_IMMEDIATE)
               ? names[g_vsyncctl.mode] : "unknown";
    n = snprintf(buf, len, "vsyncctl mode=%s hz=%u interval=%ums tear=%d frames=%lu",
                 name, g_vsyncctl.refresh_hz, g_vsyncctl.interval_ms,
                 g_vsyncctl.tearing_allowed,
                 (unsigned long)g_vsyncctl.present_count);
    if (n < 0 || (size_t)n >= len) return -1;
    return n;
}
