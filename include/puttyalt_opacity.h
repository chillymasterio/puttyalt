/*
 * puttyalt_opacity.h: Window transparency control.
 *
 * Set terminal window opacity (0-100%) for overlaying on other
 * content.  Adjustable with Ctrl+Shift+scroll.
 */

#ifndef PUTTYALT_OPACITY_H
#define PUTTYALT_OPACITY_H

typedef struct OpacityConfig {
    int  opacity_pct;       /* 10-100, default 100 (fully opaque) */
    int  inactive_opacity;  /* opacity when window loses focus */
    int  scroll_adjust;     /* allow Ctrl+Shift+scroll to change */
    int  step;              /* percent per scroll notch */
} OpacityConfig;

void opacity_init(OpacityConfig *oc);
void opacity_increase(OpacityConfig *oc);
void opacity_decrease(OpacityConfig *oc);
int  opacity_get_current(const OpacityConfig *oc, int is_active);

#endif /* PUTTYALT_OPACITY_H */
